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

#include "Blaster.h"

#include "AppearEnemy.h"

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
	objPipeline.reset(KGPlin::Create(objShader, "Obj"));

	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline.reset(KGPlin::Create(spriteShader, "Sprite"));
}

void GameScence::Init()
{
	// インスタンス
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
	player.reset(Player::Create(playerModel.get(),objPipeline.get()));
	player->SetParent(&camera->GetTransform());

	// 雑魚敵出現パターン読み込み
	LoadEnemyPopData();

	// 地面
	ground = std::make_unique<Ground>();
	ground->Init();
}

void GameScence::Update()
{
	ImGui::Begin("Camera");
	ImGui::Text("CameraPos(%f,%f,%f)", camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);
	ImGui::End();

	ImGui::Begin("Ground");
	ImGui::Text("Grand1Pos(%f,%f,%f)", ground->GetPos(0).x, ground->GetPos(0).y, ground->GetPos(0).z);
	ImGui::Text("Grand2Pos(%f,%f,%f)", ground->GetPos(1).x, ground->GetPos(1).y, ground->GetPos(1).z);
	ImGui::End();

	// ボスバトル開始判定
	BossBattleStart();

	// 敵出現
	UpdateEnemyPopCommands();

	// 当たり判定
	CheckAllCollisions();

	if (input->IsTrigger(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	// 敵消去
	mobEnemys.remove_if([](std::unique_ptr<MobEnemy>& MobEnemy)
		{
			return MobEnemy->GetIsDead();
		});

	// プレイヤーの更新
	player->Update(camera->GetViewPro());

	// 雑魚敵の更新
	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
	{
		mobEnemy->Update(camera->GetViewPro(), camera->GetPos());
	}

	// ボスの更新
	if (boss)
	{
		boss->Update(camera->GetViewPro());
	}

	// 地面の更新
	ground->Update(camera->GetViewPro(), camera->GetPos());

	// カメラの更新
	camera->Update(player.get());

	// ボス登場警告
	if (bossWarning)
	{
		bossWarning->Update();
	}
}

void GameScence::Draw()
{
	// 地面描画
	ground->Draw();

	// 雑魚敵描画
	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
	{
		mobEnemy->Draw();
	}

	// ボス描画
	if (boss)
	{
		boss->Draw();
	}

	// プレイヤー描画
	player->Draw();

	// ボス登場警告演出
	if (bossWarning)
	{
		bossWarning->Draw();
	}

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
			std::unique_ptr<MobEnemy> newMEnemy;
			newMEnemy.reset(AppearEnemy::Create(mobEnemysModel.get(), objPipeline.get(), { x,y,z }));
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

void GameScence::BossBattleStart()
{
	// ボスバトルが始まってればスキップ
	if (isBossBattle) { return; }

	// ボスバトル開始座標
	const float bossBattleStartPos = 3200;

	if (!bossWarning)
	{
		bool isBossBattleStart = camera->GetPos().z >= bossBattleStartPos;
		if (!isBossBattleStart) { return; }

		// カメラ前進止める
		camera->SetIsAdvance(false);

		// ボス登場警告作成
		bossWarning = std::make_unique<Warning>();
		bossWarning->Init();
	}
	else
	{
		// 演出が終わってないときは抜ける
		if (!bossWarning->GetIsDelete()) { return; }

		// ボス登場警告解放
		bossWarning.reset();

		// ボス配置
		const float bossDistance = 150;
		const KMyMath::Vector3 bossBasePos = { 0.0f, 23.0f, bossBattleStartPos + bossDistance };
		boss.reset(Blaster::Create(mobEnemysModel.get(), objPipeline.get(), bossBasePos));

		// ボスバトル開始
		isBossBattle = true;
	}
}
