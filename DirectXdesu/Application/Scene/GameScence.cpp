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

#include "Ease.h"

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
	player.reset(Player::Create(playerModel.get(),
		objPipeline.get(),
		10,
		spritePipeline.get()
	));

	// カメラ生成
	camera = std::make_unique<RailCamera>();

	// シーンマネージャーインスタンス
	sceneManager = SceneManager::GetInstance();

	// カメラ初期化
	camera->Init(player.get(), { 0,0,-200 });

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
	skyBox.reset(SkyBox::Create(skyBoxModel.get(),
		objPipeline.get(),
		50
	));

	bulletManager = BulletManager::GetInstance();
	bulletManager->Init(playersBulletModel.get(),
		objPipeline.get());

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
	if (isStageStart)
	{
		billManager->SetIsStopCreate(true);

		StageStartMovie();
	}
	else
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

		// エネミーマネージャーの更新
		enemyManager->Update(camera->GetViewPro(), camera->GetCameraPos());

		// ボスの更新
		if (boss)
		{
			boss->Update(camera->GetViewPro());
		}

		// 天箱を自機に追従
		skyBox->SetPosZ(player->GetWorldPos().z);
	}

	// プレイヤーの更新
	player->Update(camera->GetViewPro(), isStageStart);

	// 弾の更新
	bulletManager->Update(camera->GetViewPro());

	// 地面の更新
	ground->Update(camera->GetViewPro(), camera->GetCameraPos());

	// スカイボックスの更新
	skyBox->Update(camera->GetViewPro());

	// パーティクルマネージャーの更新
	particleManager->Update(camera->GetViewPro());
	objParticleManager->Update(camera->GetViewPro());

	// ビルマネージャー
	billManager->Update(camera->GetViewPro(), camera->GetCameraPos().z);

	// カメラの更新
	camera->Update(isStageStart);

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

	// モブエネミー描画
	enemyManager->Draw();

	// ボス描画
	if (boss)
	{
		boss->Draw();
	}

	// プレイヤー描画
	player->ObjDraw();

	// スカイボックス描画
	skyBox->ObjDraw();

	// 弾の描画
	bulletManager->Draw();

	// パーティクルの描画
	particleManager->Draw();
	objParticleManager->Draw();

	// 建物描画
	billManager->Draw();
}

void GameScence::SpriteDraw()
{
	// ボス登場警告演出
	if (bossWarning)
	{
		bossWarning->Draw();
	}

	// UI描画
	player->UIDraw();

	if (boss)
	{
		// ボスUI描画
		boss->UIDraw();
	}

	// 2D情報描画
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
		bool isBossBattleStart = camera->GetCameraPos().z >= bossBattleStartPos;
		if (!isBossBattleStart) { return; }

		// カメラ前進止める
		camera->SetIsAdvance(false);

		// ビルを全部動かす状態へ
		billManager->SetIsAdvance(true);

		// 地面を動かす状態へ
		ground->SetIsAdvance(true);

		// ボス登場警告作成
		bossWarning = std::make_unique<Warning>();
		bossWarning->Init();

		if (!boss)
		{
			// ボス配置
			const float bossDistance = 150;
			const KMyMath::Vector3 bossBasePos =
			{
				0.0f,
				23.0f,
				bossBattleStartPos + bossDistance
			};

			// 生成
			boss.reset(Blaster::Create(mobEnemysModel.get(),
				objPipeline.get(),
				bossBasePos,
				10,
				spritePipeline.get()
			));
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

		if (bossWarning)
		{
			// ボス出現演出前の暗転開始
			sceneChange->Start();

			// ボス登場警告解放
			bossWarning.reset();
		}

		if (sceneChange->GetIsChange())
		{
			isBossAppearEffect = true;
			bulletManager->AllBulletDelete();
		}

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

void GameScence::StageStartMovie()
{
	// カメラワーク一段階(上から見下ろし)
	if (startPhase == 0)
	{
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0)
		{
			//自機とカメラの距離
			const KMyMath::Vector3 playerDistance = { 0.0f, 20.0f, 40.0f };

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			camera->SetCameraRot({ 20.0f,180.0f,camera->GetCameraRot().z });

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			camera->SetCameraPos(
				{ camera->GetCameraPos().x,
				MyEase::Lerp(player->GetWorldPos().y + 20.0f,player->GetWorldPos().y + 10.0f,startPhaseTimer / startPhaseTime),
				MyEase::Lerp(player->GetWorldPos().z + 40.0f,player->GetWorldPos().z + 45.0f,startPhaseTimer / startPhaseTime) }
			);

			camera->SetCameraRot(
				{ MyEase::Lerp(20.0f,10.0f,startPhaseTimer / startPhaseTime) ,
				camera->GetCameraRot().y,
				camera->GetCameraRot().z
				}
			);
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// カメラワーク二段階(自機右上から至近距離)
	else if (startPhase == 1)
	{
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0)
		{
			//自機とカメラの距離
			const KMyMath::Vector3 playerDistance = { 2.5f, 2.5f, 3.5f };

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			camera->SetCameraRot({ 45.0f,250.0f,camera->GetCameraRot().z });

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			camera->SetCameraPos(
				{ camera->GetCameraPos().x,
				camera->GetCameraPos().y,
				MyEase::Lerp(player->GetWorldPos().z + 3.5f,player->GetWorldPos().z + 1.5f,startPhaseTimer / startPhaseTime) }
			);
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// カメラワーク三段階(自機左したからブースター(ケツ)注視)
	else if (startPhase == 2)
	{
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0)
		{
			//自機とカメラの距離
			const KMyMath::Vector3 playerDistance = { -2.0f, -1.8f, -3.5f };

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			camera->SetCameraRot({ -35.0f,27.5f,camera->GetCameraRot().z });

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			camera->SetCameraPos(
				{ MyEase::Lerp(player->GetWorldPos().x - 2.0f,player->GetWorldPos().x - 1.5f,startPhaseTimer / startPhaseTime),
				camera->GetCameraPos().y,
				camera->GetCameraPos().z }
			);

			camera->SetCameraRot(
				{ camera->GetCameraRot().x,
				MyEase::Lerp(35.0f,27.5f,startPhaseTimer / startPhaseTime),
				camera->GetCameraRot().z }
			);
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// カメラワーク四段階(正面でカメラを引く)
	else if (startPhase == 3)
	{
		startPhaseTime = 120.0f;

		if (startPhaseTimer == 0)
		{
			//自機とカメラの距離
			const KMyMath::Vector3 playerDistance = { 0.0f, 1.0f, 10.0f };

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			camera->SetCameraRot({ 0.0f,180.0f,camera->GetCameraRot().z });

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			camera->SetCameraPos(
				{ camera->GetCameraPos().x,
				camera->GetCameraPos().y,
				MyEase::OutCubicFloat(player->GetWorldPos().z + 10.0f,player->GetWorldPos().z + 30.0f,startPhaseTimer / startPhaseTime) }
			);
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// 自機が向かってくる
	else if (startPhase == 4)
	{
		startPhaseTime = 60;

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			player->SetPos({ player->GetPosition().x,
				player->GetPosition().y,
				MyEase::OutCubicFloat(50.0f,100.0f,startPhaseTimer / startPhaseTime) });
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	else
	{
		billManager->SetIsStopCreate(false);
		camera->EndStart();
		player->EndStart();
		// 親子関係接続
		player->SetParent(&camera->GetTransform());
		isStageStart = false;
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

void GameScence::BossAppearEffect()
{
	if (!isBossAppearEffect)
	{
		return;
	}


}

const bool GameScence::GetIsStart() const
{
	return isStageStart;
}
