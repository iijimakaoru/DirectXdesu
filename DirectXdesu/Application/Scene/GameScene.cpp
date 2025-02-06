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

void GameScene::LoadResources() {
	// モデル
	objModel[OBJ::stage] = 
		ModelManager::GetInstance()->GetModels("S_Cube");
	objModel[OBJ::skydome] = 
		ModelManager::GetInstance()->GetModels("S_SkyDorm");
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
	obj[OBJ::skydome]->SetColor({ 0.1f,0.0f,1.0f,1.0f });

	resetPos = { -50.0f,50.0f,10.0f };

	for (size_t i = 0; i < Hand::max; i++)
	{
		KMyMath::Vector4 color;
		KMyMath::Vector3 pos;
		KMyMath::Vector3 scale;
		handObj[i].reset(KObject3d::Create(objModel[OBJ::stage],PipelineManager::GetInstance()->GetPipeline("Obj")));
		if (i==0)
		{
			color = { 0.5f,0.2f,0.2f,1.0f };
			handObj[i]->SetColor(color);
		}
		else
		{
			color = { 0.2f,0.3f,1.0f,1.0f };
			handObj[i]->SetColor(color);
		}
		pos = resetPos;
		pos.x += 100.0f*i;
		scale = { 5.0f,5.0f,5.0f };
		handObj[i]->GetTransform().SetPos(pos);
		handObj[i]->GetTransform().SetScale(scale);

	}

	collisionManager_ = new CollisionManager();

	//ノーツ
	playTime = 0;
	Meter meter = { 3,4 };
	music = std::make_unique<MusicDesc>(85.0f, meter);

	noteObj = std::make_unique<NoteObj>();
	noteObj->Init(music.get());

	lenRimit = 100.0f;//csvに落とし込む,値を仮設定
	move = { 2.0f,2.0f,0.0f };//仮で設定

	light_->SetLightRGB({lightRGB_.x, lightRGB_.y, lightRGB_.z});
	light_->SetLightDir({lightDir_.x, lightDir_.y, lightDir_.z, 0.0f});
}

void GameScene::Update() {
	
	float l[2] = { input->GetPadLStick().x,input->GetPadLStick().y };
	float r[2] = { input->GetPadRStick().x,input->GetPadRStick().y };
	ImGui::Begin("lo");
	ImGui::DragFloat2("L", l, ImGuiColorEditFlags_Float);
	ImGui::DragFloat2("R",r, ImGuiColorEditFlags_Float);
	ImGui::End();

	playTime++;
	OutPutCollision();
	for (size_t i = 0; i < Hand::max; i++)
	{
		Hand hand = static_cast<Hand>(i);
		KMyMath::Vector3 pos = PosHand(hand);
		handObj[i]->GetTransform().SetPos({ pos });
	}
	for (size_t i = 0; i < OBJ::max; i++)
	{
		obj[i]->Update(camera->GetViewPro(), camera->GetWorldPos());
	}

	noteObj->Update(camera.get());

	for (size_t i = 0; i < Hand::max; i++) 
	{
		handObj[i]->Update(camera->GetViewPro(), camera->GetWorldPos());
	}

	camera->Update();

}

void GameScene::ObjDraw() 
{
	for (size_t i = 0; i < OBJ::max; i++) 
	{
		obj[i]->Draw();
	}
	for (size_t i = 0; i < Hand::max; i++)
	{
		handObj[i]->Draw();
	}

	noteObj->Draw();
}

void GameScene::SpriteDraw() {
	
}

void GameScene::Final() 
{
	delete collisionManager_; 
}

KMyMath::Vector3 GameScene::PosHand(Hand hand_)
{
	KMyMath::Vector2 len;
	KMyMath::Vector3 pos;
	if (hand_==Hand::L)
	{
		len = input->GetPadLStick();
	}
	else
	{
		len = input->GetPadRStick();
	}
	pos = handObj[hand_]->GetTransform().GetPos();
	pos.x += move.x * len.x;
	pos.y += move.y * len.y;

	return pos;
}

void GameScene::RotAndLenCalculationMouse()
{
	//end = input->GetMousePos();

	//KMyMath::Vector2 mouseVec = { 0.0f,0.0f };
	////ウィンドウの中心点とマウスの現在点のベクトルをとる
	//mouseVec.x = end.x - start.x;
	//mouseVec.y = end.y - start.y;
	////長さ算出
	//length = MyMathUtility::Vector2Length(mouseVec);
	////正規化
	//mouseVec = MyMathUtility::MakeVector2Normalize(mouseVec);
	////角度を算出
	//angle = atan2(mouseVec.y, mouseVec.x);
	//angle = MyMathConvert::DegreeTransform(angle);
}

void GameScene::RotAndLenCalculationStick(Hand hand_)
{
	end[hand_] = handObj[hand_]->GetTransform().GetPos();

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
	float scope = 45.0f;
	float center=0.0f;
	float max, min;
	int lane = 0;
	bool isSuccess = false;
	auto ago = noteObj->Notes().begin();
	for (auto it = noteObj->Notes().begin(); it != noteObj->Notes().end(); ++it)
	{
		//フラグが立っているなら次のノードへ
		if (noteObj->Notes()[it->first].isHit)
		{
			continue;
		}
		//ノードと現在のタイムを比較
		float notetime = sec * music->ConvertBeatToMiliSeconds(noteObj->Notes()[it->first].beat);
		float diff = notetime - playTime;
		//スタート位置の取得10から5フレーム前に取得
		if (diff<= perfect +10&&diff>= perfect+5)
		{
			lane = noteObj->Notes()[it->first].lane;
			if (lane==0)//左
			{
				start[lane] = handObj[lane]->GetTransform().GetPos();
			}
			else if(lane==1)//右
			{
				start[lane] = handObj[lane]->GetTransform().GetPos();
			}
		}
		
		//コントローラ、マウス
		if (std::abs(diff) <= perfect)
		{
			//1個前のノードのフラグが立っていないかつ同じレーンじゃない場合にしなければならない
			if (it != noteObj->Notes().begin())
			{
				if (!noteObj->Notes()[ago->first].isHit)
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
				lenRimit = 5.0f;//仮
				lane = noteObj->Notes()[it->first].lane;
				if (lane == 0)
				{
					RotAndLenCalculationStick(static_cast<Hand>(0));
				}
				else if (lane == 1)
				{
					RotAndLenCalculationStick(static_cast<Hand>(1));
				}
			}


			if (noteObj->Notes()[it->first].direction == DIRECTION::right)
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
			else if (noteObj->Notes()[it->first].direction == DIRECTION::up)
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
			else if (noteObj->Notes()[it->first].direction == DIRECTION::dawn)
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
			else if (noteObj->Notes()[it->first].direction == DIRECTION::left)
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
				KMyMath::Vector3 pos;
				combo++;
				pos = resetPos;
				pos.x += 100.0f * lane;
				handObj[lane]->GetTransform().SetPos(pos);
				noteObj->Notes()[it->first].isHit = true;
			}
			break;//for文から抜ける
		}
		else if (diff < -(perfect))//一旦ノードがラインから過ぎ去ったらミスにする
		{
			combo = 0;
			score[MISS]++;
			noteObj->Notes()[it->first].isHit = true;
		}
		ago = it;
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
	auto ago = noteObj->Notes().begin();
	for (auto it = noteObj->Notes().begin(); it != noteObj->Notes().end(); ++it)
	{
		//フラグが立っているなら次のノードへ
		if (noteObj->Notes()[it->first].isHit)
		{
			continue;
		}
		//ノードと現在のタイムを比較
		float notetime = sec * music->ConvertBeatToMiliSeconds(noteObj->Notes()[it->first].beat);
		float diff = notetime - playTime;
		//スタート位置の取得10から5フレーム前に取得
		if (diff <= perfect + 10 && diff >= perfect + 5)
		{
			lane = noteObj->Notes()[it->first].lane;
			if (lane == 0)//左
			{
				start[lane] = handObj[lane]->GetTransform().GetPos();
			}
			else if (lane == 1)//右
			{
				start[lane] = handObj[lane]->GetTransform().GetPos();
			}
		}

		//コントローラ、マウス
		if (std::abs(diff) <= perfect)
		{
			//1個前のノードのフラグが立っていないかつ同じレーンじゃない場合にしなければならない
			if (it != noteObj->Notes().begin())
			{
				if (!noteObj->Notes()[ago->first].isHit)
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
				lenRimit = 5.0f;//仮
				lane = noteObj->Notes()[it->first].lane;
				if (lane == 0)
				{
					RotAndLenCalculationStick(static_cast<Hand>(0));
				}
				else if (lane == 1)
				{
					RotAndLenCalculationStick(static_cast<Hand>(1));
				}
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

				noteObj->Notes()[it->first].direction = DIRECTION::right;
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
				noteObj->Notes()[it->first].direction = DIRECTION::up;
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
				noteObj->Notes()[it->first].direction = DIRECTION::dawn;
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
				noteObj->Notes()[it->first].direction = DIRECTION::left;
				isSuccess = true;

			}
			
			if (isSuccess)
			{
				KMyMath::Vector3 pos;
				combo++;
				pos = resetPos;
				pos.x += 100.0f * lane;
				handObj[lane]->GetTransform().SetPos(pos);
				noteObj->Notes()[it->first].isHit = true;
			}
			break;//for文から抜ける
		}
		else if (diff < -(perfect))//一旦ノードがラインから過ぎ去ったらミスにする
		{
			combo = 0;
			score[MISS]++;
			noteObj->Notes()[it->first].isHit = true;
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
