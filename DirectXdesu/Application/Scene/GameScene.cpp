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
	/*ステージ*/
	obj[OBJ::stage].reset(KObject3d::Create(objModel[OBJ::stage],
		PipelineManager::GetInstance()->GetPipeline("Obj")));
	obj[OBJ::stage]->GetTransform().SetPos({ 0.0f,0.0f,200.0f });
	obj[OBJ::stage]->GetTransform().SetScale({ 100.0f,1.0f,300.0f });
	obj[OBJ::stage]->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	/*ライン*/
	float scaleZ = perfect;
	obj[OBJ::line].reset(KObject3d::Create(objModel[OBJ::stage],
		PipelineManager::GetInstance()->GetPipeline("Obj")));
	obj[OBJ::line]->GetTransform().SetScale({ 100.0f,1.0f,scaleZ });
	obj[OBJ::line]->GetTransform().SetPos({ 0.0f,4.0f,0.0f });
	obj[OBJ::line]->SetColor({ 0.8f,0.8f,0.8f,1.0f });

	/*天球*/
	obj[OBJ::skydome].reset(KObject3d::Create(objModel[OBJ::skydome], 
		PipelineManager::GetInstance()->GetPipeline("Obj")));
	obj[OBJ::skydome]->GetTransform().SetScale({ 800.0f, 800.0f, 800.0f });
	obj[OBJ::skydome]->SetColor({ 0.1f,0.0f,1.0f,1.0f });

	resetPos = { -50.0f,50.0f,10.0f };

	
	collisionManager_ = new CollisionManager();

	//ノーツ
	playTime = 0;
	Meter meter = { 4,4 };
	music = std::make_unique<MusicDesc>(85.0f, meter);
	test = false;
	noteObj = std::make_unique<NoteObj>();
	noteObj->Init(test,music.get());

	lenRimit = 100.0f;//csvに落とし込む,値を仮設定
	move = { 2.0f,2.0f,0.0f };//仮で設定

	light_->SetLightRGB({lightRGB_.x, lightRGB_.y, lightRGB_.z});
	light_->SetLightDir({lightDir_.x, lightDir_.y, lightDir_.z, 0.0f});
	//------------------------------------------------------------------------------------------------------------------------------------------------------------//


	PHONONLOADER::P_MODEL_DATA* pData = new PHONONLOADER::P_MODEL_DATA();
	PHONONLOADER::PModelLoader::Load(pData, "obj/cube");

	

	cv::Mat img;

	const std::string& modelPath = "Resources/Checkpoints/yolo11x-pose.onnx";

	float mask_threshold = 0.5f;
	float conf_threshold = 0.30f;
	float iou_threshold = 0.45f;
	int conversion_code = cv::COLOR_BGR2RGB;
	
	MCBM::AnimationModelManager::GetInstance()->Load("fox");
	player = std::make_unique<CaptureModel>();
	player->Initilize("fox");
	sprite.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	texData = TextureManager::GetInstance()->GetTextures("Resources/texture/boss1.png");

	playerTrans.SetPos({ 0,49,-147 });
	playerTrans.SetRot({ 0,180,0 });
	player->Update(camera->GetViewPro(), playerTrans);
	frame = 0;
	isFrame = false;
}

void GameScene::Update() {
	
	player->Update(camera->GetViewPro(), playerTrans);
	
	if (input->IsTrigger(DIK_SPACE))isFrame = true;
		
	if (isFrame)
	{
		if (frame<360)
		{
			player->InitializePose();
		}
		else
		{
			isFrame = false;
			initialePoseSet = true;
			frame = 0;
		}
		frame++;
	}

	if (input->GetMouseClickTrigger(MouseBotton::Left))
	{
		start[1] = player->GetCapturePos(YOLO_POSE_INDEX::WRIST_R);
		start[1].y -= start[1].y * 2.0f;

	}
	if (input->GetMouseClickTrigger(MouseBotton::Right))
	{
		RotAndLenCalculationStick(Hand::R);
	}
	float a[2] = { start[1].x,start[1].y };
	float b[2] = { end[1].x,end[1].y };
	ImGui::Begin("lo");
	
	ImGui::InputFloat2("start", a);
	ImGui::InputFloat2("end", b);
	ImGui::InputFloat("angle", &angle);
	ImGui::InputFloat("len", &length);

	ImGui::End();

	if (test)OutPutCollision();
	else Collision();
	if (initialePoseSet)
	{
		playTime++;

		for (size_t i = 0; i < OBJ::max; i++)
		{
			obj[i]->Update(camera->GetViewPro(), camera->GetWorldPos());
		}

		noteObj->Update(camera.get());
	}

	camera->Update();

}

void GameScene::ObjDraw() 
{
	for (size_t i = 0; i < OBJ::max; i++) 
	{
		obj[i]->Draw();
	}
	/*for (size_t i = 0; i < Hand::max; i++)
	{
		handObj[i]->Draw();
	}*/

	noteObj->Draw();
	player->Draw();
}

void GameScene::SpriteDraw() {

	//------------------------------------------------------------------------------------------------------------------------------------------------------------//
	f++;
	fDiv = 7;
	//sprite->AnimationDraw(texData, 64, 64, f, fDiv, {200,200});
}

void GameScene::Final() 
{
	delete collisionManager_; 
}

void GameScene::RotAndLenCalculationStick(Hand hand_)
{
	if (hand_==Hand::L)	end[hand_] = player->GetCapturePos(YOLO_POSE_INDEX::WRIST_L);
	else end[hand_] = player->GetCapturePos(YOLO_POSE_INDEX::WRIST_R);
	
	end[hand_].y -= end[hand_].y * 2.0f;
	KMyMath::Vector2 stickVec = { 0.0f,0.0f };

	//スタート位置からの現在点のベクトルをとる
	stickVec.x = end[hand_].x - start[hand_].x;
	stickVec.y = end[hand_].y - start[hand_].y;
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
	float scope = 60.0f;
	float center=0.0f;
	float max, min;
	int lane = 0;
	bool isSuccess = false;
	for (size_t i = 0; i < noteObj->Notes().size(); i++)
	{
		//フラグが立っているなら次のノードへ
		if (noteObj->Notes()[i].isHit)
		{
			continue;
		}
		//ノードと現在のタイムを比較
		float notetime = sec * music->ConvertBeatToMiliSeconds(noteObj->Notes()[i].beat);
		float diff = notetime - playTime;
		//スタート位置の取得10から5フレーム前に取得
		if (diff<= perfect +10&&diff>= perfect+5)
		{
			lane = noteObj->Notes()[i].lane;
			if (lane==0)//左
			{
				start[lane] = player->GetCapturePos(YOLO_POSE_INDEX::WRIST_L);
				start[0].y -= start[0].y * 2.0f;

			}
			else if(lane==1)//右
			{
				start[lane] = player->GetCapturePos(YOLO_POSE_INDEX::WRIST_R);
				start[0].y -= start[0].y * 2.0f;

			}
		}
		
		//コントローラ、マウス
		if (std::abs(diff) <= perfect)
		{
			//1個前のノードのフラグが立っていないかつ同じレーンじゃない場合にしなければならない
			if (i != 0)
			{
				if (!noteObj->Notes()[i-1].isHit)
				{
					continue;
				}
			}
			lenRimit = 20.0f;//仮
			lane = noteObj->Notes()[i].lane;
			if (lane == 0)
			{
				RotAndLenCalculationStick(static_cast<Hand>(0));
			}
			else if (lane == 1)
			{
				RotAndLenCalculationStick(static_cast<Hand>(1));
			}

			if (noteObj->Notes()[i].direction == DIRECTION::right)
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
			else if (noteObj->Notes()[i].direction == DIRECTION::up)
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
			else if (noteObj->Notes()[i].direction == DIRECTION::dawn)
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
			else if (noteObj->Notes()[i].direction == DIRECTION::left)
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
			if (isSuccess)
			{
				combo++;
				noteObj->Notes()[i].isHit = true;
			}
			break;//for文から抜ける
		}
		else if (diff < -(perfect))//一旦ノードがラインから過ぎ去ったらミスにする
		{
			combo = 0;
			score[MISS]++;
			noteObj->Notes()[i].isHit = true;
		}
	}
}

void GameScene::OutPutCollision()
{
	//範囲の指定（一応45と設定）
	float scope = 45.0f;
	float center = 0.0f;
	float max, min;
	int lane = 0;
	bool isSuccess = false;
	auto ago = noteObj->NotesMap().begin();
	for (auto it = noteObj->NotesMap().begin(); it != noteObj->NotesMap().end(); ++it)
	{
		//フラグが立っているなら次のノードへ
		if (noteObj->NotesMap()[it->first].isHit)
		{
			continue;
		}
		//ノードと現在のタイムを比較
		float notetime = sec * music->ConvertBeatToMiliSeconds(noteObj->NotesMap()[it->first].beat);
		float diff = notetime - playTime;
		//スタート位置の取得5から1フレーム前に取得
		if (diff <= perfect + 5 && diff >= perfect + 1)
		{
			lane = noteObj->NotesMap()[it->first].lane;
			if (lane == 0)//左
			{
				start[lane] = player->GetCapturePos(YOLO_POSE_INDEX::WRIST_L);
				start[0].y -= start[0].y * 2.0f;

			}
			else if (lane == 1)//右
			{
				start[lane] = player->GetCapturePos(YOLO_POSE_INDEX::WRIST_R);
				start[0].y -= start[0].y * 2.0f;
			}
		}

		//コントローラ、マウス
		if (std::abs(diff) <= perfect)
		{
			//1個前のノードのフラグが立っていないかつ同じレーンじゃない場合にしなければならない
			if (it != noteObj->NotesMap().begin())
			{
				if (!noteObj->NotesMap()[ago->first].isHit)
				{
					continue;
				}
			}
			lenRimit = 20.0f;//仮
			lane = noteObj->NotesMap()[it->first].lane;
			if (lane == 0)
			{
				RotAndLenCalculationStick(static_cast<Hand>(0));
			}
			else if (lane == 1)
			{
				RotAndLenCalculationStick(static_cast<Hand>(1));
			}

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

				noteObj->NotesMap()[it->first].direction = DIRECTION::right;
				isSuccess = true;
			}
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
				noteObj->NotesMap()[it->first].direction = DIRECTION::up;
				isSuccess = true;
			}

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
				noteObj->NotesMap()[it->first].direction = DIRECTION::dawn;
				isSuccess = true;
			}
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
				noteObj->NotesMap()[it->first].direction = DIRECTION::left;
				isSuccess = true;

			}
			
			if (isSuccess)
			{
				combo++;
				noteObj->NotesMap()[it->first].isHit = true;
			}
			break;//for文から抜ける
		}
		else if (diff < -(perfect))//一旦ノードがラインから過ぎ去ったらミスにする
		{
			combo = 0;
			score[MISS]++;
			noteObj->NotesMap()[it->first].isHit = true;
		}
		ago = it;
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
