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

GameScene::~GameScene() { Final(); };

void GameScene::LoadResources() 
{
	ModelManager* modelManager = ModelManager::GetInstance();

	// モデル
	objModel[OBJ::stage] = 
		modelManager->GetModels("S_Cube");
	objModel[OBJ::skydome] = 
		modelManager->GetModels("S_SkyDorm");
	noteModel = 
		modelManager->GetModels("S_Arrow");
}

void GameScene::Init() 
{
	timer_ = Timer(KWinApp::GetHWND(), KWinApp::GetWindow().lpszMenuName);

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
	obj[OBJ::skydome]->SetColor({ 0.1f,0.0f,1.0f,1.0f });

	collisionManager_ = new CollisionManager();

	// エフェクトの初期化
	effectManager = std::make_unique<EffectManager>();
	effectManager->Init(timer_, camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro());

	//ノーツ
	playTime = 0;
	Meter meter = { 3,4 };
	music = std::make_unique<MusicDesc>(85.0f, meter);
	notes.push_back({ { 1,1,4 },1 ,DIRECTION::right });
	notes.push_back({ { 1,2,4 },0 ,DIRECTION::left });
	notes.push_back({ { 2,0,4 },1 ,DIRECTION::right });
	notes.push_back({ { 2,1,4 },0 ,DIRECTION::up });
	notes.push_back({ { 2,2,4 },0 ,DIRECTION::right });
	notes.push_back({ { 2,2,4 },1 ,DIRECTION::left });
	notes.push_back({ { 3,0,4 },1 ,DIRECTION::dawn });
	notes.push_back({ { 3,1,4 },0 ,DIRECTION::right });
	notes.push_back({ { 3,2,4 },1 ,DIRECTION::left });
	notes.push_back({ { 4,0,4 },1 ,DIRECTION::up });
	notes.push_back({ { 4,0,4 },0 ,DIRECTION::up });
	notes.push_back({ { 4,1,4 },0 ,DIRECTION::right });
	notes.push_back({ { 4,2,4 },0 ,DIRECTION::right });
	notes.push_back({ { 5,0,4 },1 ,DIRECTION::right });
	notes.push_back({ { 5,1,4 },0 ,DIRECTION::dawn });
	notes.push_back({ { 5,1,4 },1 ,DIRECTION::dawn });
	notes.push_back({ { 5,2,4 },0 ,DIRECTION::left });
	notes.push_back({ { 6,0,4 },1 ,DIRECTION::dawn });
	notes.push_back({ { 6,1,4 },0 ,DIRECTION::right });
	notes.push_back({ { 6,2,4 },1 ,DIRECTION::up });
	notes.push_back({ { 7,0,4 },0 ,DIRECTION::dawn });
	notes.push_back({ { 7,0,4 },1 ,DIRECTION::dawn });
	notes.push_back({ { 7,1,4 },1 ,DIRECTION::up });
	notes.push_back({ { 7,2,4 },0 ,DIRECTION::left });
	notes.push_back({ { 8,0,4 },1 ,DIRECTION::right });
	notes.push_back({ { 8,1,4 },0 ,DIRECTION::right });
	notes.push_back({ { 8,1,4 },1 ,DIRECTION::left });
	notes.push_back({ { 8,2,4 },1 ,DIRECTION::dawn });
	notes.push_back({ { 9,0,4 },0 ,DIRECTION::right });
	notes.push_back({ { 9,1,4 },1 ,DIRECTION::right });
	notes.push_back({ { 9,2,4 },0 ,DIRECTION::left });
	notes.push_back({ { 10,0,4 },1 ,DIRECTION::up });
	notes.push_back({ { 10,0,4 },0 ,DIRECTION::dawn });
	notes.push_back({ { 10,1,4 },0 ,DIRECTION::right });
	notes.push_back({ { 10,2,4 },1 ,DIRECTION::left });
	notes.push_back({ { 11,0,4 },0 ,DIRECTION::right });
	notes.push_back({ { 11,1,4 },0 ,DIRECTION::right });
	notes.push_back({ { 11,1,4 },1 ,DIRECTION::left });
	notes.push_back({ { 11,2,4 },0 ,DIRECTION::left });
	notes.push_back({ { 12,0,4 },1 ,DIRECTION::right });
	notes.push_back({ { 12,1,4 },0 ,DIRECTION::right });
	notes.push_back({ { 12,2,4 },0 ,DIRECTION::left });
	notes.push_back({ { 12,2,4 },1 ,DIRECTION::right });

	for (size_t i = 0; i < notes.size(); i++)
	{
		std::unique_ptr<KObject3d> obj_;
		obj_.reset(KObject3d::Create(noteModel, 
			PipelineManager::GetInstance()->GetPipeline("Obj")));
		obj_->GetTransform().SetScale({ 15.0f,15.0f,5.0f });

		//色設定
		if (notes[i].lane == 0)
		{
			obj_->SetColor({ 0.5f,0.0f,0.0f,1.0f });
		}
		else
		{
			obj_->SetColor({ 0.0f,0.3f,1.0f,1.0f });
		}

		//方向設定
		if (notes[i].direction == DIRECTION::left)//左
		{
			obj_->GetTransform().SetRot({ 0.0f,180.0f,0.0f });
		}
		else if (notes[i].direction == DIRECTION::up)//上
		{
			obj_->GetTransform().SetRot({ 0.0f,0.0f,-90.0f });
		}
		else if (notes[i].direction == DIRECTION::dawn)//下
		{
			obj_->GetTransform().SetRot({ 0.0f,180.0f,90.0f });
		}
		else										//右
		{
			obj_->GetTransform().SetRot({ 0.0f,0.0f,0.0f });
		}
		notePosZ = (sec * speed) * music->ConvertBeatToMiliSeconds(notes[i].beat);
		obj_->GetTransform().SetPos({ -50.0f + (100.0f * notes[i].lane),25.0f,notePosZ });

		objNote.push_back(std::move(obj_));
	}

	start = { 500,500 };
	lenRimit = 100.0f;//csvに落とし込む,値を仮設定

	// 音
	audioManager_ = AudioManager::GetInstance();
	audioManager_->BGMPlay_wav("maou_bgm_cyber44.wav");
}

void GameScene::Update() 
{
	ImGui::Begin("lo");
	ImGui::DragInt("perfect", &score[PERFECT]);
	ImGui::DragInt("great", &score[GREAT]);
	ImGui::DragInt("miss", &score[MISS]);
	ImGui::DragInt("combo", &combo);
	ImGui::End();

	timer_.UpdateTimer();

	light_->SetLightRGB({lightRGB_.x, lightRGB_.y, lightRGB_.z});
	light_->SetLightDir({lightDir_.x, lightDir_.y, lightDir_.z, 0.0f});

	//角度算出
	RotAndLenCalculationStick(input->GetPadLStick());

	playTime++;
	Collision();

	for (size_t i = 0; i < OBJ::max; i++)
	{
		obj[i]->Update(camera->GetViewPro(), camera->GetWorldPos());
	}

	for (size_t i = 0; i < objNote.size(); i++)
	{
		if (!notes[i].isHit)
		{
			KMyMath::Vector3 move;
			move = objNote[i]->GetTransform().GetPos();
			move.z -= speed;

			objNote[i]->GetTransform().SetPos(move);
			objNote[i]->Update(camera->GetViewPro(), camera->GetWorldPos());
		}
	}

	for (size_t i = 0; i < OBJ::max; i++) 
	{
		obj[i]->Update(camera->GetViewPro(), camera->GetWorldPos());
	}

	// エフェクトの更新
	effectManager->Update(timer_, camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro());

	camera->Update();
}

void GameScene::ObjDraw() 
{
	for (size_t i = 0; i < OBJ::max; i++) 
	{
		obj[i]->Draw();
	}

	for (size_t i = 0; i < objNote.size(); i++)
	{
		if (!notes[i].isHit)
		{
			objNote[i]->Draw();
		}
	}

	// エフェクト描画
	effectManager->Draw(timer_, camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro());
}

void GameScene::SpriteDraw() 
{
	
}

void GameScene::Final() { delete collisionManager_; }

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

void GameScene::RotAndLenCalculationStick(KMyMath::Vector2 vec)
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

	for (size_t i = 0; i < notes.size(); i++)
	{
		//フラグが立っているなら次のノードへ
		if (notes[i].isHit)
		{
			continue;
		}
		//ノードと現在のタイムを比較
		float notetime = sec * music->ConvertBeatToMiliSeconds(notes[i].beat);
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
				if (!notes[i - 1].isHit)
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

				if (notes[i].lane == 0)
				{
					RotAndLenCalculationStick(input->GetPadLStick());
				}
				else if (notes[i].lane == 1)
				{
					RotAndLenCalculationStick(input->GetPadRStick());
				}
			}


			if (notes[i].direction == DIRECTION::right)
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
			else if (notes[i].direction == DIRECTION::up)
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
			else if (notes[i].direction == DIRECTION::dawn)
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
			else if (notes[i].direction == DIRECTION::left)
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
				notes[i].isHit = true;

				// エフェクト発生
				KMyMath::Vector3 nowPos = objNote[i]->GetTransform().GetPos();
				KMyMath::Vector3 nowRot = objNote[i]->GetTransform().GetRot();
				KMyMath::Vector3 nowScale = objNote[i]->GetTransform().GetScale();
				KMyMath::Vector4 nowColor = objNote[i]->GetColor();
				effectManager->SetArrowEffect(nowPos, nowRot, nowScale, nowColor,
					timer_, camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro());
			}
			break;//for文から抜ける
		}
		else if (diff < -(perfect))//一旦ノードがラインから過ぎ去ったらミスにする
		{
			combo = 0;
			score[MISS]++;
			notes[i].isHit = true;
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
