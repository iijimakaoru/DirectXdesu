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
#include "FlyEnemy.h"
#include "CanonEnemy.h"

GameScence::~GameScence()
{
	Final();
};

void GameScence::LoadResources()
{
	// テクスチャ
	textureData = TextureManager::Load("Resources/texture/mario.jpg");
	textureData2 = TextureManager::Load("Resources/texture/kariPlayerColor.png");

	// モデル
	playerModel = std::make_unique<MtlObj>("BattleShip");
	playerModel->CreateModel();
	mobEnemysModel = std::make_unique<MtlObj>("boss_model");
	mobEnemysModel->CreateModel();
	playersBulletModel = std::make_unique<MtlObj>("playerBullet");
	playersBulletModel->CreateModel();
	skyBoxModel = std::make_unique<MtlObj>("SkyBox");
	skyBoxModel->CreateModel();

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

	// fbx
	fbxShader.Init(L"Resources/Shader/FbxVS.hlsl", L"Resources/Shader/FbxPS.hlsl");
	fbxPipeline.reset(KGPlin::Create(fbxShader, "Fbx"));
}

void GameScence::Init()
{
	BaseScene::Init();

	// インスタンス
	input = KInput::GetInstance();

	// プレイヤー生成
	player.reset(Player::Create(playerModel.get(), objPipeline.get(), 10, spritePipeline.get()));

	// カメラ生成
	camera = std::make_unique<RailCamera>();

	sceneManager = SceneManager::GetInstance();

	// カメラ初期化
	camera->Init(player.get());

	// 親子関係
	player->SetParent(&camera->GetTransform());

	// 雑魚敵出現パターン読み込み
	LoadEnemyPopData();

	// 地面
	ground = std::make_unique<Ground>();
	ground->Init();

	// スカイボックス
	skyBox.reset(SkyBox::Create(skyBoxModel.get(), objPipeline.get(), player->GetWorldPos().z));

	bulletManager = BulletManager::GetInstance();
	bulletManager->Init(playersBulletModel.get(), objPipeline.get());

	// パーティクル
	particleManager = ParticleManager::GetInstance();
	particleManager->Init();

	objParticleManager = ObjParticleManager::GetInstance();
	objParticleManager->Init();

	billManager = std::make_unique<BillManager>();
	billManager->Init();

	isCallDeadCamera = false;
}

void GameScence::Update()
{
	// ボスバトル開始判定
	BossBattleStart();

	// 敵出現
	UpdateEnemyPopCommands();

	// 当たり判定
	CheckAllCollisions();

	// ゲームクリアシーンへの移動
	if (boss)
	{
		if (boss->GetIsFallEffectEnd())
		{
			goClearSceneTimer++;
			if (goClearSceneTimer == goClearSceneTime)
			{
				sceneChange->Start();
				goClearSceneTimer = goClearSceneTime + 1.0f;
			}

			if (sceneChange->GetIsChange())
			{
				sceneManager->ChangeScene("CLEAR");
				bulletManager->AllBulletDelete();
			}
		}
	}

	// ゲームオーバーへの移動
	if (player)
	{
		if (player->GetIsFallEffectEnd())
		{
			goOverSceneTimer++;
			if (goOverSceneTimer == goOverSceneTime)
			{
				sceneChange->Start();
				goOverSceneTimer = goOverSceneTime + 1.0f;
			}

			if (sceneChange->GetIsChange())
			{
				sceneManager->ChangeScene("OVER");
				bulletManager->AllBulletDelete();
			}
		}
	}

	// 敵消去
	mobEnemys.remove_if([](std::unique_ptr<MobEnemy>& MobEnemy)
		{
			return MobEnemy->GetIsDead();
		});

	if (player->GetIsDead() && !isCallDeadCamera)
	{
		camera->CallCrash();
		isCallDeadCamera = true;
		player->SetParent(nullptr);
		player->SetPos(player->GetWorldPos());
	}

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
	else
	{
		billManager->Set(camera->GetPos().z);
	}

	// 弾の更新
	bulletManager->Update(camera->GetViewPro());

	// 地面の更新
	ground->Update(camera->GetViewPro(), camera->GetPos());

	// スカイボックスの更新
	skyBox->SetPosZ(player->GetWorldPos().z);
	skyBox->Update(camera->GetViewPro());

	// パーティクルマネージャーの更新
	particleManager->Update(camera->GetViewPro());
	objParticleManager->Update(camera->GetViewPro());

	// ビルマネージャー
	billManager->Update(camera->GetViewPro(), camera->GetPos().z);

	// カメラの更新
	camera->Update();

	// ボス登場警告
	if (bossWarning)
	{
		bossWarning->Update();
	}
}

void GameScence::ObjDraw()
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
	player->ObjDraw();

	skyBox->ObjDraw();

	bulletManager->Draw();

	particleManager->Draw();
	objParticleManager->Draw();

	billManager->Draw();
}

void GameScence::SpriteDraw()
{
	// ボス登場警告演出
	if (bossWarning)
	{
		bossWarning->Draw();
	}

	player->UIDraw();

	if (boss)
	{
		boss->UIDraw();
	}

	player->SpriteDraw();
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
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = bulletManager->GetPlayerBullets();

	// 敵弾の取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bulletManager->GetEnemyBullets();

	// 自弾と敵の当たり判定
	{
		for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
		{
			if (mobEnemy)
			{
				// 敵の座標
				posA = mobEnemy->GetWorldPos();

				for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
				{
					// 弾の座標
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

	// 敵弾と自機の当たり判定
	if (!player->GetIsDead() && !player->GetIsInvisible())
	{
		// 自機の座標
		posA = player->GetWorldPos();

		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
		{
			if (bullet)
			{
				// 弾の座標
				posB = bullet->GetWorldPos();

				// 球同士の交差判定
				if (MyCollisions::CheckSphereToSphere(posA, posB, 3, 2))
				{
					// 弾消去
					bullet->OnCollision();

					// 自機被弾処理
					player->OnCollision();
				}
			}
		}
	}

	// ボスと自弾の判定
	{
		if (boss && isBossBattle)
		{
			if (!boss->GetIsDead())
			{
				// ボスの座標
				posA = boss->GetWorldPos();

				for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
				{
					// 弾の座標
					posB = bullet->GetWorldPos();

					// 球同士の交差判定
					if (boss->CollisionCheck(posA, posB))
					{
						// 弾消去
						bullet->OnCollision();

						// 敵消去
						boss->OnCollision();
					}
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
			// 敵の種類
			getline(line_stream, word, ',');
			size_t enemyType = static_cast<size_t>(std::atof(word.c_str()));

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

			// 該当するタイプの敵生成
			if (enemyType == MobEnemy::EnemysType::Fly)
			{
				// x座標
				getline(line_stream, word, ',');
				float _x = static_cast<float>(std::atof(word.c_str()));

				// y座標
				getline(line_stream, word, ',');
				float _y = static_cast<float>(std::atof(word.c_str()));

				newMEnemy.reset(FlyEnemy::Create(mobEnemysModel.get(), objPipeline.get(), { x,y,z }, { _x,_y }, camera->GetSpeed()));
				newMEnemy->SetPlayer(player.get());
				// 登録
				mobEnemys.push_back(std::move(newMEnemy));
			}
			else if (enemyType == MobEnemy::EnemysType::Canon)
			{
				newMEnemy.reset(CanonEnemy::Create(mobEnemysModel.get(), objPipeline.get(), { x,y,z }));
				newMEnemy->SetPlayer(player.get());
				// 登録
				mobEnemys.push_back(std::move(newMEnemy));
			}
			else if (enemyType == MobEnemy::EnemysType::Appear)
			{
				newMEnemy.reset(AppearEnemy::Create(mobEnemysModel.get(), objPipeline.get(), { x,y,z }));
				newMEnemy->SetPlayer(player.get());
				// 登録
				mobEnemys.push_back(std::move(newMEnemy));
			}
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
	const float bossBattleStartPos = 500;

	if (!bossWarning)
	{
		bool isBossBattleStart = camera->GetPos().z >= bossBattleStartPos;
		if (!isBossBattleStart) { return; }

		// カメラ前進止める
		camera->SetIsAdvance(false);

		// ボス登場警告作成
		bossWarning = std::make_unique<Warning>();
		bossWarning->Init();

		if (!boss)
		{
			// ボス配置
			const float bossDistance = 150;
			const KMyMath::Vector3 bossBasePos = { 0.0f, 23.0f, bossBattleStartPos + bossDistance };
			boss.reset(Blaster::Create(mobEnemysModel.get(), objPipeline.get(), bossBasePos,
				10, spritePipeline.get()));
		}
	}
	else
	{
		if (boss)
		{
			if (boss->GetIsHPE())
			{
				boss->HPGauge(bossWarning->GetTime());
			}
		}

		// 演出が終わってないときは抜ける
		if (!bossWarning->GetIsDelete()) { return; }

		// ボス登場警告解放
		bossWarning.reset();

		// ボスバトル開始
		isBossBattle = true;
	}
}
