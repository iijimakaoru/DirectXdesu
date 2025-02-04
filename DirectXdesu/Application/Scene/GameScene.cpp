#include "GameScene.h"
#include "KInput.h"

#include "MyMath.h"

#include "DebugCamera.h"
#include "GameCamera.h"

#include "Collision.h"
#include <iomanip>
#include <sstream>

#include "FbxLoader.h"

#include "SceneManager.h"
#include "Ease.h"

#include "PipelineManager.h"

#include "ModelManager.h"
#include"Vector2.h"

#include<PModelLoader.h>

GameScene::~GameScene() { Final(); };

void GameScene::LoadResources() {
	// モデル
	objModel[OBJ::stage] = 
		ModelManager::GetInstance()->GetModels("S_Cube");
	objModel[OBJ::skydome] = 
		ModelManager::GetInstance()->GetModels("S_SkyDorm");
	/*noteModel = 
		ModelManager::GetInstance()->GetModels("S_Arrow");*/

	TextureManager::Load("Resources/texture/boss1.png");


}

void GameScene::Init() {

	BaseScene::Init();

	LoadCSV("collision");

	// インスタンス
	input = KInput::GetInstance();

	light_.reset(Light::Create());
	light_->SetLightRGB({1, 1, 1});
	KObject3d::SetLight(light_.get());

	// カメラ生成
	camera = std::make_unique<GameCamera>();
	camera->Init();

	// シーンマネージャーインスタンス
	sceneManager = SceneManager::GetInstance();

	// モデル
	obj[OBJ::stage].reset(KObject3d::Create(objModel[OBJ::stage],
		PipelineManager::GetInstance()->GetPipeline("Obj")));
	obj[OBJ::stage]->GetTransform().SetPos({ 0.0f,0.0f,200.0f });
	obj[OBJ::stage]->GetTransform().SetScale({ 100.0f,1.0f,300.0f });
	obj[OBJ::stage]->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	float scaleZ = perfect;
	obj[OBJ::line].reset(KObject3d::Create(objModel[OBJ::stage],
		PipelineManager::GetInstance()->GetPipeline("Obj")));
	obj[OBJ::line]->GetTransform().SetScale({ 100.0f,1.0f,scaleZ });
	obj[OBJ::line]->GetTransform().SetPos({ 0.0f,4.0f,0.0f });
	obj[OBJ::line]->SetColor({ 0.8f,0.8f,0.8f,1.0f });

	obj[OBJ::skydome].reset(KObject3d::Create(objModel[OBJ::skydome], 
		PipelineManager::GetInstance()->GetPipeline("Obj")));
	obj[OBJ::skydome]->GetTransform().SetScale({ 800.0f, 800.0f, 800.0f });
	obj[OBJ::skydome]->GetTransform().SetPos({ 0.0f, 100.0f, 0.0f });

	collisionManager_ = new CollisionManager();

	//ノーツ
	playTime = 0;
	Meter meter = { 3,4 };
	music = std::make_unique<MusicDesc>(85.0f, meter);

	noteObj = std::make_unique<NoteObj>();
	noteObj->Init(music.get());

	start = { 500,500 };
	lenRimit = 100.0f;//csvに落とし込む,値を仮設定

	//------------------------------------------------------------------------------------------------------------------------------------------------------------//


	PHONONLOADER::P_MODEL_DATA* pData = new PHONONLOADER::P_MODEL_DATA();
	PHONONLOADER::PModelLoader::Load(pData, "obj/cube");

	cap= cv::VideoCapture(0, cv::CAP_DSHOW);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 600);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

	if (!cap.isOpened())
	{
		assert(0);
	}

	cv::Mat img;

	const std::string& modelPath = "Resources/Checkpoints/yolo11x-pose.onnx";

	float mask_threshold = 0.5f;
	float conf_threshold = 0.30f;
	float iou_threshold = 0.45f;
	int conversion_code = cv::COLOR_BGR2RGB;

	m_YOLOPoseEstimation.reset(CreateYOLOPoseEstimation());

	m_YOLOPoseEstimation->CameraInitialize(&cap);

	m_YOLOPoseEstimation->ModelInitialize(modelPath.c_str(), mask_threshold, conf_threshold, iou_threshold,ONNXP_ROVIDERS::DIRECTML);

	m_YOLOPoseEstimation->Start(true);

	sprite.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	texData = TextureManager::GetInstance()->GetTextures("Resources/texture/boss1.png");
}

void GameScene::Update() {

	
	light_->SetLightRGB({lightRGB_.x, lightRGB_.y, lightRGB_.z});
	light_->SetLightDir({lightDir_.x, lightDir_.y, lightDir_.z, 0.0f});

	playTime++;
	Collision();

	for (size_t i = 0; i < OBJ::max; i++)
	{
		obj[i]->Update(camera->GetViewPro(), camera->GetWorldPos());
	}

	noteObj->Update(camera.get());

	for (size_t i = 0; i < OBJ::max; i++) 
	{
		obj[i]->Update(camera->GetViewPro(), camera->GetWorldPos());
	}

	obj[OBJ::skydome]->GetTransform().SetRot({ 0.0f, playTime * 0.05f, 0.0f });

	camera->Update();
}

void GameScene::ObjDraw() 
{
	for (size_t i = 0; i < OBJ::max; i++) 
	{
		obj[i]->Draw();
	}

	noteObj->Draw();
}

void GameScene::SpriteDraw() {

	//------------------------------------------------------------------------------------------------------------------------------------------------------------//
	f++;
	fDiv = 7;
	sprite->AnimationDraw(texData, 64, 64, f, fDiv, {200,200});
}

void GameScene::Final() 
{
	delete collisionManager_; 
}

void GameScene::RotAndLenCalculationMouse()
{
	end = input->GetMousePos();

	KMyMath::Vector2 mouseVec = { 0.0f,0.0f };
	//ウィンドウの中心点とマウスの現在点のベクトルをとる
	mouseVec.x = end.x - start.x;
	mouseVec.y = end.y - start.y;
	//長さ算出
	length = MyMathUtility::Vector2Length(mouseVec);
	//正規化
	mouseVec = MyMathUtility::MakeVector2Normalize(mouseVec);
	//角度を算出
	angle = atan2(mouseVec.y, mouseVec.x);
	angle = MyMathConvert::DegreeTransform(angle);
}

void GameScene::RotAndLenCalculationStick(KMyMath::Vector2& vec)
{
	end = vec;

	KMyMath::Vector2 stickVec = { 0.0f,0.0f };
	KMyMath::Vector2 s = { 0.0f,0.0f };

	//ウィンドウの中心点とマウスの現在点のベクトルをとる
	stickVec.x = end.x - s.x;
	stickVec.y = end.y - s.y;
	//長さ算出
	length = MyMathUtility::Vector2Length(stickVec);
	//正規化
	stickVec = MyMathUtility::MakeVector2Normalize(stickVec);
	//角度を算出
	angle = atan2(stickVec.y, stickVec.x);
	angle = MyMathConvert::DegreeTransform(angle);
}

void GameScene::Collision()
{
	//範囲の指定（一応45と設定）
	float scope = 45.0f;
	float center;
	bool isSuccess = false;
	float max, min;

	for (size_t i = 0; i < noteObj->Notes().size(); i++)
	{
		//フラグが立っているなら次のノードへ
		if (noteObj->Notes()[i]->isHit)
		{
			continue;
		}
		//ノードと現在のタイムを比較
		float notetime = sec * music->ConvertBeatToMiliSeconds(noteObj->Notes()[i]->beat);
		float diff = notetime - playTime;
		//60
		if (diff <= 20 || !input->GetPadConnect())
		{
			start = input->GetMousePos();
		}
		//コントローラ、マウス
		if (std::abs(diff) <= perfect)
		{
			//1個前のノードのフラグが立っていないかつ同じ位置じゃない場合にしなければならない
			if (i != 0)
			{
				if (!noteObj->Notes()[i - 1]->isHit)
				{
					continue;
				}
			}
			if (!input->GetPadConnect())
			{
				RotAndLenCalculationMouse();
				lenRimit = 100.0f;//仮
			}
			else
			{
				lenRimit = 0.7f;//仮

				if (noteObj->Notes()[i]->lane == 0)
				{
					KMyMath::Vector2 rot = input->GetPadLStick();
					RotAndLenCalculationStick(rot);
				}
				else if (noteObj->Notes()[i]->lane == 1)
				{
					KMyMath::Vector2 rot = input->GetPadRStick();
					RotAndLenCalculationStick(rot);
				}
			}


			if (noteObj->Notes()[i]->direction == DIRECTION::right)
			{
				center = 0;
				min = center - scope;
				max = center + scope;
				if (min <= angle && angle <= max)
				{
					//長さが一定以上超えていないなら
					if (length < lenRimit)
					{
						continue;
					}

					score[PERFECT]++;
					isSuccess = true;
				}

			}
			else if (noteObj->Notes()[i]->direction == DIRECTION::up)
			{
				center = -90;
				min = center - scope;
				max = center + scope;
				if (min <= angle && angle <= max)
				{
					//長さが一定以上超えていないなら
					if (length < lenRimit)
					{
						continue;
					}
					score[PERFECT]++;
					isSuccess = true;
				}

			}
			else if (noteObj->Notes()[i]->direction == DIRECTION::dawn)
			{
				center = 90;
				min = center - scope;
				max = center + scope;
				if (min <= angle && angle <= max)
				{
					//長さが一定以上超えていないなら
					if (length < lenRimit)
					{
						continue;
					}
					score[PERFECT]++;
					isSuccess = true;
				}

			}
			else if (noteObj->Notes()[i]->direction == DIRECTION::left)
			{
				center = 180;
				min = -(center - scope);
				max = center - scope;
				if (max <= angle || angle <= min)
				{
					//長さが一定以上超えていないなら
					if (length < lenRimit)
					{
						continue;
					}
					score[PERFECT]++;
					isSuccess = true;
				}

			}
			if (isSuccess)
			{
				combo++;
				noteObj->Notes()[i]->isHit = true;
			}
			break;//for文から抜ける
		}
		else if (diff < -(perfect))//一旦ノードがラインから過ぎ去ったらミスにする
		{
			combo = 0;
			score[MISS]++;
			noteObj->Notes()[i]->isHit = true;
		}
	}
}

void GameScene::LoadCSV(const std::string& name)
{
	//ファイルを開く
	const std::string filename = "Resources/csv/" + name + ".csv";
	std::ifstream file;
	file.open(filename);
	assert(file.is_open());


	//1行分の文字列を入れる変数
	std::string line;

	while (std::getline(file, line))
	{
		std::istringstream line_stream(line);
		//,区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ',');
		if (key == "perfect")
		{
			line_stream >> perfect;
		}
		else if (key == "minusShift")
		{
			line_stream >> minusShift;
		}
		else if (key == "great")
		{
		}

	}
	//ファイルを閉じる
	file.close();
}
