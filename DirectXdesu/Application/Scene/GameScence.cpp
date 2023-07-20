#include "GameScence.h"
#include "KInput.h"

#include "MyMath.h"

#include "DebugCamera.h"
#include "GameCamera.h"

#include "Collision.h"
#include <sstream>
#include <iomanip>

#include "FbxLoader.h"

#include "SceneManager.h"

GameScence::~GameScence()
{
	Final();
};

void GameScence::LoadResources()
{
	// テクスチャ
	textureData = TextureManager::Load("Resources/texture/mario.jpg");

	// モデル
	playerModel = std::make_unique<MtlObj>("kariBattle");
	playerModel->CreateModel();
	mobEnemysModel = std::make_unique<MtlObj>("boss_model");
	mobEnemysModel->CreateModel();

	// サウンド
	soundData1 = Sound::GetInstance()->SoundLoadWave("Resources/Sound/Alarm01.wav");
	soundData2 = Sound::GetInstance()->SoundLoadWave("Resources/Sound/Alarm02.wav");
	soundData3 = Sound::GetInstance()->SoundLoadWave("Resources/Sound/Alarm03.wav");

	// パイプライン
	// Obj
	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	objPipeline = std::make_unique<KGPlin>();
	objPipeline->CreatePipelineAll(objShader, "Obj");

	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");
}

void GameScence::Init()
{
	input = KInput::GetInstance();

#pragma region スプライト
	sprite = std::make_unique<Sprite>();
	sprite->Init();
	sprite->SetPipeline(spritePipeline.get());
#pragma endregion

	isDebug = true;
	camera = std::make_unique<RailCamera>();

	sceneManager = SceneManager::GetInstance();

	// プレイヤー
	player = std::make_unique<Player>();
	player->Init(playerModel.get());
	const WorldTransfom& cameraToPlayer = camera->GetTransform();
	player->SetParent(&cameraToPlayer);

	// 雑魚敵出現パターン読み込み
	LoadEnemyPopData();

	// 地面
	ground = std::make_unique<Ground>();
	ground->Init();
}

void GameScence::Update()
{
	if (input->IsTrigger(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	ImGui::Text("Sound");
	if (ImGui::Button("Play1"))
	{
		Sound::GetInstance()->SoundPlayWave(soundData1);
	}
	if (ImGui::Button("Play2"))
	{
		Sound::GetInstance()->SoundPlayWave(soundData2);
	}
	if (ImGui::Button("Play3"))
	{
		Sound::GetInstance()->SoundPlayWave(soundData3);
	}

	ImGui::Text("Sprite");
	ImGui::Text("pos: (%.2f,%.2f)", spritePos.x, spritePos.y);
	ImGui::SliderFloat("SpritePosX", &spritePos.x, 0.0f, 100.0f);
	ImGui::SliderFloat("SpritePosY", &spritePos.y, 0.0f, 100.0f);
	ImGui::Text("size: (%.2f,%.2f)", spriteSize.x, spriteSize.y);
	ImGui::SliderFloat("SpriteSizeX", &spriteSize.x, 0.0f, 100.0f);
	ImGui::SliderFloat("SpriteSizeY", &spriteSize.y, 0.0f, 100.0f);
	ImGui::Text("rot: (%.2f)", spriteRot);
	ImGui::SliderFloat("SpriteRot", &spriteRot, 0.0f, 180.0f);
	ImGui::Text("color: (%.2f,%.2f,%.2f,%.2f)", spriteColor.x, spriteColor.y, spriteColor.z, spriteColor.w);
	ImGui::SliderFloat("SpriteColorR", &spriteColor.x, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteColorG", &spriteColor.y, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteColorB", &spriteColor.z, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteColorA", &spriteColor.w, 0.0f, 1.0f);
	ImGui::Text("CameraAdvance");
	if (ImGui::Button("ON"))
	{
		camera->SetIsAdvance(true);
	}
	if (ImGui::Button("OFF"))
	{
		camera->SetIsAdvance(false);
	}

	ImGui::Begin("Player");
	ImGui::Text("pos: (%.2f,%.2f, %.2f)", player->GetPosition().x,
		player->GetPosition().y, player->GetPosition().z);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::Text("pos: (%.2f,%.2f, %.2f)", camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);
	ImGui::End();

	// 敵出現
	UpdateEnemyPopCommands();

	// 敵消去
	mobEnemys.remove_if([](std::unique_ptr<MobEnemy>& MobEnemy)
		{
			return MobEnemy->GetIsDead();
		});

	// 当たり判定
	CheckAllCollisions();

	player->Update(camera->GetViewPro());

	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
	{
		mobEnemy->Update(camera->GetViewPro(), camera->GetPos());
	}

	ground->Update(camera->GetViewPro());

	camera->Update(player.get());
}

void GameScence::Draw()
{
	// 地面描画
	ground->Draw();

	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
	{
		mobEnemy->Draw();
	}

	// プレイヤー描画
	player->Draw();

	sprite->Draw(textureData,spritePos, spriteSize,spriteRot, spriteColor, spriteFlipX, spriteFlipY);
}

void GameScence::Final()
{
	Sound::GetInstance()->GetxAudio().Reset();
	Sound::GetInstance()->SoundUnLoad(&soundData1);
	Sound::GetInstance()->SoundUnLoad(&soundData2);
	Sound::GetInstance()->SoundUnLoad(&soundData3);
}

void GameScence::CheckAllCollisions()
{
	// 判定対象AとBの座標
	KMyMath::Vector3 posA, posB;

	// 自機弾の取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

	// 自弾と敵の当たり判定
	{
		for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
		{
			// 敵の座標
			posA = mobEnemy->GetWorldPos();

			for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
			{
				posB = bullet->GetWorldPos();

				// 球同士の交差判定
				if (MyCollisions::CheckSphereToSphere(posA, posB, 6, 2))
				{
					// 弾消去
					bullet->OnCollision();

					// 敵消去
					mobEnemy->OnCollision();
				}
			}
		}
	}
}

void GameScence::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScence::UpdateEnemyPopCommands()
{
	// 待機処理
	if (isStand)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			// 待機終了
			isStand = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line))
	{
		// 1行分の文字数をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// コメント
		if (word.find("//") == 0)
		{
			// 行を飛ばす
			continue;
		}

		// POP
		if (word.find("POP") == 0)
		{
			// x座標
			getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// y座標
			getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// z座標
			getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			/// 敵を発生させる
			// 生成
			std::unique_ptr<MobEnemy> newMEnemy = std::make_unique<MobEnemy>();
			// 初期化
			newMEnemy->Init(mobEnemysModel.get());
			// セット
			KMyMath::Vector3 pos = { x,y,z };
			newMEnemy->Set(pos);
			// 登録
			mobEnemys.push_back(std::move(newMEnemy));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isStand = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}
