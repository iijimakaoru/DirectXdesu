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

	// シーンマネージャーインスタンス
	sceneManager = SceneManager::GetInstance();

	// カメラ初期化
	camera->Init(player.get(), { 0,0,-200 });

	// 親子関係
	player->SetParent(&camera->GetTransform());

	// エネミーマネージャー生成
	enemyManager.reset(EnemyManager::Create("Resources/csv/enemyPop.csv", // ステージのcsvを読み込む
		player.get(), // プレイヤー情報
		mobEnemysModel.get(), // モデル渡し(マネージャー内で作るか悩み中)
		objPipeline.get() // パイプライン渡し
	));

	// 地面
	ground = std::make_unique<Ground>();
	ground->Init(player.get());

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

	isStageStart = true;
}

void GameScence::Update()
{
	// ボスバトル開始判定
	BossBattleStart();

	// 当たり判定
	CheckAllCollisions();

	// ゲームクリアシーンへの移動
	GoClearScene();

	// ゲームオーバーへの移動
	GoGameOverScene();

	// 自機が死んだとき
	PlayerDead();

	// プレイヤーの更新
	player->Update(camera->GetViewPro());

	// エネミーマネージャーの更新
	enemyManager->Update(camera->GetViewPro(), camera->GetPos());

	// ボスの更新
	if (boss)
	{
		boss->Update(camera->GetViewPro());
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

	// エネミーマネージャー描画
	enemyManager->Draw();

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

	// 敵の取得
	const std::list<std::unique_ptr<MobEnemy>>& mobEnemys = enemyManager->GetMobEnemys();

	// 自弾と敵の当たり判定
	{
		for (const std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
		{
			if (!mobEnemy)
			{
				return;
			}

			// 敵の座標
			posA = mobEnemy->GetWorldPos();

			for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
			{
				if (!bullet)
				{
					return;
				}

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

	// 敵弾と自機の当たり判定
	{
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
		{
			// 弾がないor自機が死んでるor自機が無敵状態の時はスキップ
			if (!bullet || player->GetIsDead() || player->GetIsInvisible())
			{
				return;
			}

			// 弾の座標
			posA = bullet->GetWorldPos();

			posB = player->GetWorldPos();

			// 球同士の交差判定
			if (MyCollisions::CheckSphereToSphere(posA, posB, 2, 3))
			{
				// 弾消去
				bullet->OnCollision();

				// 自機被弾処理
				player->OnCollision();
			}
		}
	}

	// 自弾とボスの判定
	{
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
		{
			// 弾が無いorボスがいないorボスが死んでるorボスバトルが始まってなかったらスキップ
			if (!bullet || !boss || boss->GetIsDead() || !isBossBattle)
			{
				return;
			}

			// 弾の座標
			posA = bullet->GetWorldPos();

			// ボスの座標
			posB = boss->GetWorldPos();

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

void GameScence::BossBattleStart()
{
	// ボスバトルが始まってればスキップ
	if (isBossBattle)
	{
		return;
	}

	// ボスバトル開始座標
	const float bossBattleStartPos = 500;

	if (!bossWarning)
	{
		bool isBossBattleStart = camera->GetPos().z >= bossBattleStartPos;
		if (!isBossBattleStart) { return; }

		// カメラ前進止める
		camera->SetIsAdvance(false);

		// ビル生成止め
		billManager->SetIsStopCreate(true);

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

		// ビル生成再開
		billManager->SetIsStopCreate(false);

		// ビルを全部動かす状態へ
		billManager->SetIsAdvance(true);

		// 地面を動かす状態へ
		ground->SetIsAdvance(true);

		// ボスバトル開始
		isBossBattle = true;
	}
}

void GameScence::PlayerDead()
{
	if (player->GetIsDead() && !isCallDeadCamera)
	{
		camera->CallCrash();
		isCallDeadCamera = true;
		player->SetParent(nullptr);
		player->SetPos(player->GetWorldPos());
	}
}

void GameScence::GoClearScene()
{
	if (!boss)
	{
		return;
	}

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

void GameScence::GoGameOverScene()
{
	if (!player)
	{
		return;
	}

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
