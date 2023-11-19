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
	movieBarTex = TextureManager::Load("Resources/texture/white1x1.png");

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
	camera->Init(player.get(), { 0.0f,0.0f,-200.0f });
	//camera->Init(player.get(), { 0.0f,0.0f,450.0f });

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

	// 弾マネージャー
	bulletManager = BulletManager::GetInstance();
	bulletManager->Init(playersBulletModel.get(),
		objPipeline.get());

	// パーティクル
	particleManager = ParticleManager::GetInstance();
	particleManager->Init();
	objParticleManager = ObjParticleManager::GetInstance();
	objParticleManager->Init();

	// ビル
	billManager = std::make_unique<BillManager>();
	billManager->Init();

	for (size_t i = 0; i < 2; i++)
	{
		movieBar[i] = std::make_unique<Sprite>();
		movieBar[i]->Init();
		movieBar[i]->SetPipeline(spritePipeline.get());
	}

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
	else if (isBossAppearMovie)
	{
		BossAppearMovie();
	}
	else if (isClearMovie)
	{
		ClearMovie();
	}
	else if (isOverMovie)
	{
		GoGameOverScene();
	}
	else
	{
		// ボスバトル開始判定
		BossBattleStart();

		// 当たり判定
		CheckAllCollisions();

		// 自機が死んだとき
		PlayerDead();

		// エネミーマネージャーの更新
		enemyManager->Update(camera->GetViewPro());

		// 天箱を自機に追従
		skyBox->SetPosZ(player->GetWorldPos().z);
	}

	// ボスの更新
	if (boss)
	{
		if (boss->GetIsFallEffectEnd())
		{
			goClearMovieTimer++;
			if (goClearMovieTimer == goClearMovieTime)
			{
				player->SetParent(nullptr);
				player->SetPos(player->GetWorldPos());
				isClearMovie = true;
				goClearMovieTimer = goClearMovieTime + 1.0f;
			}
		}

		boss->Update(camera->GetViewPro(), isBossAppearMovie);
	}

	// プレイヤーの更新
	player->Update(camera->GetViewPro(), isStageStart, isBossAppearMovie, isClearMovie);

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
	billManager->Update(camera->GetViewPro(), camera->GetCameraPos().z - 20.0f);

	// カメラの更新
	camera->Update(isStageStart, isBossAppearMovie, isClearMovie);

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

	if (isOverMovie || isClearMovie || isStageStart || isBossAppearMovie)
	{
		for (size_t i = 0; i < 2; i++)
		{
			movieBar[0]->Draw(movieBarTex,
				movieBarPos[0],
				{ static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW()),50.0f },
				0.0f,
				{ 0.0f,0.0f,0.0f,1.0f },
				false, false,
				{ 0.0f,0.0f });

			movieBar[1]->Draw(movieBarTex,
				movieBarPos[1],
				{ static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW()),50.0f },
				0.0f,
				{ 0.0f,0.0f,0.0f,1.0f },
				false, false,
				{ 0.0f,1.0f });
		}
	}
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
				const float enemyRange = 6.0f;
				const float bulletRange = 2.0f;
				if (MyCollisions::CheckSphereToSphere(posA, posB, enemyRange, bulletRange))
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
			const float bulletRange = 2.0f;
			const float playerRange = 6.0f;
			if (MyCollisions::CheckSphereToSphere(posA, posB, bulletRange, playerRange))
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
	if (isWarnning)
	{
		return;
	}

	// ボスバトル開始座標
	bossBattleStartPos = 500;

	if (!bossWarning)
	{
		bool isBossBattleStart = camera->GetCameraPos().z >= bossBattleStartPos;

		// スタート位置にいなかったらスキップ
		if (!isBossBattleStart)
		{
			return;
		}

		// カメラ前進止める
		camera->SetIsAdvance(false);

		// ビルを全部動かす状態へ
		billManager->SetIsAdvance(true);

		// 地面を動かす状態へ
		ground->SetIsAdvance(true);

		// ボス登場警告作成
		bossWarning = std::make_unique<Warning>();
		bossWarning->Init();
	}
	else
	{
		// 演出が終わってないときは抜ける
		if (!bossWarning->GetIsDelete())
		{
			return;
		}

		if (!boss)
		{
			// ボス配置
			const float bossDistance = 150;
			const KMyMath::Vector3 bossBasePos =
			{
				0.0f,
				120.0f,
				bossBattleStartPos + bossDistance
			};

			// 生成
			const float initBHP = 10.0f;
			boss.reset(Blaster::Create(mobEnemysModel.get(),
				objPipeline.get(),
				bossBasePos,
				initBHP,
				spritePipeline.get()
			));
		}

		//
		sceneChange->SceneChangeStart();

		// ボス登場警告解放
		bossWarning.reset();

		// ボス出現ムービーへ
		isBossAppearMovie = true;

		isWarnning = true;
	}

	// ボスバトルが始まってればスキップ
	if (isBossBattle || isBossAppearMovie)
	{
		return;
	}
}

void GameScence::PlayerDead()
{
	if (player->GetIsDead() && !isCallDeadCamera)
	{
		// 撃墜カメラ呼び出し
		camera->CallCrash();
		isCallDeadCamera = true;
		// プレイヤーとカメラの接続解除
		player->SetParent(nullptr);
		player->SetPos(player->GetWorldPos());
		// 全ての敵を消去
		enemyManager->AllEnemyDelete();
		// 撃墜ムービーへ
		isOverMovie = true;
	}
}

void GameScence::StageStartMovie()
{
	// スキップしよう
	if (startPhase < 5)
	{
		if (input->GetPadButtonDown(XINPUT_GAMEPAD_START))
		{
			startPhase = 5;
		}
	}

	// カメラワーク一段階(上から見下ろし)
	if (startPhase == 0)
	{
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0)
		{
			MovieBarInInit();

			//自機とカメラの距離
			const KMyMath::Vector3 playerDistance = { 0.0f, 20.0f, 40.0f };

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			const float rotX = 20.0f;
			const float rotY = 180.0f;
			camera->SetCameraRot({ rotX,rotY,camera->GetCameraRot().z });

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			const float startPosY = 20.0f;
			const float endPosY = 10.0f;

			const float startPosZ = 40.0f;
			const float endPosZ = 45.0f;

			camera->SetCameraPos(
				{ camera->GetCameraPos().x,
				MyEase::Lerp(player->GetWorldPos().y + startPosY,player->GetWorldPos().y + endPosY,startPhaseTimer / startPhaseTime),
				MyEase::Lerp(player->GetWorldPos().z + startPosZ,player->GetWorldPos().z + endPosZ,startPhaseTimer / startPhaseTime) }
			);

			const float startRotX = 20.0f;
			const float endRotX = 10.0f;

			camera->SetCameraRot(
				{ MyEase::Lerp(startRotX,endRotX,startPhaseTimer / startPhaseTime) ,
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
			const float rotX = 45.0f;
			const float rotY = 250.0f;
			camera->SetCameraRot({ rotX,rotY,camera->GetCameraRot().z });

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			const float startPosZ = 3.5f;
			const float endPosZ = 1.5f;

			camera->SetCameraPos(
				{ camera->GetCameraPos().x,
				camera->GetCameraPos().y,
				MyEase::Lerp(player->GetWorldPos().z + startPosZ,player->GetWorldPos().z + endPosZ,startPhaseTimer / startPhaseTime) }
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
			const float rotX = -35.0f;
			const float rotY = 27.5f;
			camera->SetCameraRot({ rotX,rotY,camera->GetCameraRot().z });

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			const float startPosX = 2.0f;
			const float endPosX = 1.5f;
			camera->SetCameraPos(
				{ MyEase::Lerp(player->GetWorldPos().x - startPosX,player->GetWorldPos().x - endPosX,startPhaseTimer / startPhaseTime),
				camera->GetCameraPos().y,
				camera->GetCameraPos().z }
			);

			const float startRotY = 35.0f;
			const float endRotY = 27.5f;
			camera->SetCameraRot(
				{ camera->GetCameraRot().x,
				MyEase::Lerp(startRotY,endRotY,startPhaseTimer / startPhaseTime),
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
			const float rotX = 0.0f;
			const float rotY = 180.0f;
			camera->SetCameraRot({ rotX,rotY,camera->GetCameraRot().z });

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			// ムービーバーを上下へ
			MovieBarOut(startPhaseTimer / startPhaseTime);

			const float startPosZ = 10.0f;
			const float endPosZ = 30.0f;

			camera->SetCameraPos(
				{ camera->GetCameraPos().x,
				camera->GetCameraPos().y,
				MyEase::OutCubicFloat(player->GetWorldPos().z + startPosZ,player->GetWorldPos().z + endPosZ,startPhaseTimer / startPhaseTime) }
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

			const float startPosZ = 50.0f;
			const float endPosZ = 100.0f;

			player->SetPos({ player->GetPosition().x,
				player->GetPosition().y,
				MyEase::OutCubicFloat(startPosZ,endPosZ,startPhaseTimer / startPhaseTime) });
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	else
	{
		MovieBarOutInit();
		billManager->SetIsStopCreate(false);
		camera->EndStart();
		player->EndStart();
		Player::isStartEase = true;
		// 親子関係接続
		player->SetParent(&camera->GetTransform());
		isStageStart = false;
	}
}

void GameScence::GoGameOverScene()
{
	if (player->GetIsFallEffectEnd())
	{
		isGoOverScene = true;
	}

	if (isGoOverScene)
	{
		goOverSceneTimer++;
		if (goOverSceneTimer == goOverSceneTime)
		{
			sceneChange->SceneChangeStart();
			goOverSceneTimer = goOverSceneTime + 1.0f;
		}

		if (sceneChange->GetIsChange())
		{
			sceneManager->ChangeScene("OVER");
			bulletManager->AllBulletDelete();
		}
	}
}

void GameScence::BossAppearMovie()
{
	// スキップしよう
	if (appearPhase > 0 && appearPhase < 5)
	{
		if (input->GetPadButtonDown(XINPUT_GAMEPAD_START))
		{
			appearPhaseTimer = 0;
			appearPhase = 5;
		}
	}

	// 暗転待ち時間
	if (appearPhase == 0)
	{
		appearPhaseTime = 30;

		if (appearPhaseTimer < appearPhaseTime)
		{
			appearPhaseTimer++;
		}
		else
		{
			// すべての弾削除
			bulletManager->AllBulletDelete();
			// プレイヤーとカメラの親子関係解消
			player->SetParent(nullptr);
			// 現在位置まで連れてくる
			player->SetPos({ 0.0f,0.0f, player->GetWorldPos().z });
			// 回転角度初期化
			player->SetRot({ 0.0f,0.0f,0.0f });
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ムービーフェーズ1
	else if (appearPhase == 1)
	{
		appearPhaseTime = 180.0f;

		MovieBarInInit();

		if (appearPhaseTimer < appearPhaseTime)
		{
			appearPhaseTimer++;

			// ボス回転させよう
			const float startRotY = 0.0f;
			const float endRotY = 360.0f;
			boss->SetRot({ boss->GetRot().x,MyEase::Lerp(startRotY,endRotY,appearPhaseTimer / appearPhaseTime),boss->GetRot().z });

			// ボス降下
			const float startPosY = 120.0f;
			const float endPosY = 80.0f;
			boss->SetPos({ boss->GetWorldPos().x,MyEase::Lerp(startPosY,endPosY,appearPhaseTimer / appearPhaseTime),boss->GetWorldPos().z });

			//自機とカメラの距離
			const KMyMath::Vector3 bossDistance = { 0.0f, 12.5f, -15.0f };
			//const KMyMath::Vector3 bossDistance = { 0.0f, 0.0f, -15.0f };

			// カメラの場所
			const KMyMath::Vector3 cameraPos = boss->GetWorldPos() + bossDistance;

			// 角度
			const float rotX = 15.0f;
			const float rotY = 0.0f;
			camera->SetCameraRot({ rotX,rotY,0.0f });
			//camera->SetCameraRot({ 0.0f,-0.0f,camera->GetCameraRot().z });

			// カメラ動け
			camera->SetCameraPos(cameraPos);
		}
		else
		{
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ムービーフェーズ2
	else if (appearPhase == 2)
	{
		appearPhaseTime = 180.0f;

		if (appearPhaseTimer < appearPhaseTime)
		{
			appearPhaseTimer++;

			// ボス回転させよう
			const float startBRotY = 0.0f;
			const float endBRotY = 360.0f;
			boss->SetRot({ boss->GetRot().x,MyEase::Lerp(startBRotY,endBRotY,appearPhaseTimer / appearPhaseTime),boss->GetRot().z });

			// ボス降下
			const float startBPosY = 80.0f;
			const float endBPosY = 40.0f;
			boss->SetPos({ boss->GetWorldPos().x,MyEase::Lerp(startBPosY,endBPosY,appearPhaseTimer / appearPhaseTime),boss->GetWorldPos().z });

			//自機とカメラの距離
			KMyMath::Vector3 bossDistance = { MyEase::Lerp(-12.0f,-11.5f,startPhaseTimer / startPhaseTime), 12.5f, -15.0f };

			// カメラの場所
			const KMyMath::Vector3 cameraPos = boss->GetWorldPos() + bossDistance;

			camera->SetCameraPos(cameraPos);

			const float startCRotY = 35.0f;
			const float endCRotY = 27.5f;
			camera->SetCameraRot(
				{ camera->GetCameraRot().x,
				MyEase::Lerp(startCRotY,endCRotY,startPhaseTimer / startPhaseTime),
				camera->GetCameraRot().z }
			);
		}
		else
		{
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ムービーフェーズ3
	else if (appearPhase == 3)
	{
		appearPhaseTime = 90.0f;

		if (appearPhaseTimer < appearPhaseTime)
		{
			appearPhaseTimer++;

			// ボス回転させよう
			const float startBRotY = 0.0f;
			const float endBRotY = 360.0f;
			boss->SetRot({ boss->GetRot().x,MyEase::OutCubicFloat(startBRotY,endBRotY,appearPhaseTimer / appearPhaseTime),boss->GetRot().z });

			// ボス降下
			const float startBPosY = 40.0f;
			const float endBPosY = 20.0f;
			boss->SetPos({ boss->GetWorldPos().x,MyEase::OutCubicFloat(startBPosY,endBPosY,appearPhaseTimer / appearPhaseTime),boss->GetWorldPos().z });

			//自機とカメラの距離
			KMyMath::Vector3 bossDistance = { 0.0f, 0.0f, -30.0f };

			// カメラの場所
			const KMyMath::Vector3 cameraPos =
			{
				boss->GetWorldPos().x + bossDistance.x,
				endBPosY + bossDistance.y,
				boss->GetWorldPos().z + bossDistance.z
			};

			camera->SetCameraPos(cameraPos);

			const KMyMath::Vector3 rot = { 0.0f,0.0f,0.0f };
			camera->SetCameraRot(rot);
		}
		else
		{
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ムービーフェーズ4
	else if (appearPhase == 4)
	{
		appearPhaseTime = 180.0f;

		if (appearPhaseTimer < appearPhaseTime)
		{
			appearPhaseTimer++;

			//自機とカメラの距離
			KMyMath::Vector3 bossDistance =
			{
				MyEase::InOutCubicFloat(0.0f, 10.0f,appearPhaseTimer / appearPhaseTime),
				MyEase::InOutCubicFloat(0.0f,-20.0f,appearPhaseTimer / appearPhaseTime),
				MyEase::InOutCubicFloat(-30.0f,-120.0f,appearPhaseTimer / appearPhaseTime)
			};

			// カメラの場所
			const KMyMath::Vector3 cameraPos = boss->GetWorldPos() + bossDistance;
			camera->SetCameraPos(cameraPos);

			const KMyMath::Vector3 rot =
			{
				0.0f,
				MyEase::InOutCubicFloat(0.0f,-15.0f,appearPhaseTimer / appearPhaseTime),
				MyEase::InOutCubicFloat(0.0f,15.0f,appearPhaseTimer / appearPhaseTime)
			};
			camera->SetCameraRot(rot);
		}
		else
		{
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	else if (appearPhase == 5)
	{
		appearPhaseTime = 30;

		if (appearPhaseTimer == 0)
		{
			sceneChange->SceneChangeStart();
		}

		if (appearPhaseTimer < appearPhaseTime)
		{
			appearPhaseTimer++;
		}
		else
		{
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ムービーフェーズ6
	else if (appearPhase == 6)
	{
		MovieBarOutInit();
		// ボス配置
		boss->SetPos({ boss->GetWorldPos().x,20.0f,boss->GetWorldPos().z });
		boss->SetRot({ 0.0f,0.0f,0.0f });
		// カメラ配置
		camera->SetCameraPos({ 0.0f,0.0f,bossBattleStartPos });
		camera->SetCameraRot({ 0.0f,0.0f,0.0f });
		// プレイヤーとカメラの親子関係解消
		player->SetParent(&camera->GetTransform());
		// 現在位置まで連れてくる
		player->SetPos({ 0.0f,0.0f, 50.0f });
		appearPhaseTimer = 0;
		appearPhase++;
	}
	else
	{
		// ムービー終わり
		isBossAppearMovie = false;
		// ボスバトル開始
		isBossBattle = true;
	}
}

void GameScence::BossBreakMovie()
{

}

void GameScence::ClearMovie()
{
	// スキップ
	if (clearPhase < 3)
	{
		if (input->GetPadButtonDown(XINPUT_GAMEPAD_START))
		{
			clearPhaseTimer = 0;
			clearPhase = 3;
		}
	}

	// フェーズ0
	if (clearPhase == 0)
	{
		clearPhaseTime = 30.0f;

		if (clearPhaseTimer < clearPhaseTime)
		{
			clearPhaseTimer++;

			MovieBarIn(clearPhaseTimer / clearPhaseTime);

			player->SetPos(MyEase::InCubicVec3(player->GetWorldPos(), { 0.0f,0.0f,player->GetWorldPos().z }, clearPhaseTimer / clearPhaseTime));
			player->SetRot(MyEase::InCubicVec3(player->GetRot(), { 0.0f,0.0f,0.0f }, clearPhaseTimer / clearPhaseTime));

			const KMyMath::Vector3 dhistans = { 0.0f,0.0f,-40.0f };

			const KMyMath::Vector3 playerPos = { 0.0f,0.0f,player->GetWorldPos().z };

			const KMyMath::Vector3 cameraPos = playerPos + dhistans;

			camera->SetCameraPos(MyEase::InOutCubicVec3(camera->GetCameraPos(), cameraPos, clearPhaseTimer / clearPhaseTime));

			camera->SetCameraRot({ 0.0f ,0.0f ,0.0f });
		}
		else
		{
			clearPhaseTimer = 0;
			clearPhase++;
		}
	}
	// フェーズ1
	else if (clearPhase == 1)
	{
		clearPhaseTime = 120.0f;

		if (clearPhaseTimer < clearPhaseTime)
		{
			clearPhaseTimer++;

			// 角度を変更
			float rotAngle = MyEase::InOutCubicFloat(0.0f, -60.0f, clearPhaseTimer / clearPhaseTime);

			const float radian = DirectX::XMConvertToRadians(rotAngle);
			const float distance = -30;

			const KMyMath::Vector3 cameraPos = { (distance * sinf(radian)) ,
				0.0f ,
				(distance * cosf(radian)) };
			camera->SetCameraPos(player->GetWorldPos() + cameraPos);

			camera->SetCameraRot({ 0.0f ,rotAngle ,0.0f });
		}
		else
		{
			clearPhaseTimer = 0;
			clearPhase++;
		}
	}
	// フェーズ2
	else if (clearPhase == 2)
	{
		if (clearPhaseTimer == 0)
		{
			start = { 0.0f, 0.0f, player->GetWorldPos().z };
			p1 = { -20.0f,  0.0f, player->GetWorldPos().z + 75.0f };
			p2 = { -50.0f, 75.0f, player->GetWorldPos().z + 100.0f };
			end = { -100.0f,100.0f, player->GetWorldPos().z + 125.0f };
		}

		clearPhaseTime = 180.0f;

		if (clearPhaseTimer < clearPhaseTime)
		{
			clearPhaseTimer++;

			// ポイント１の制御点
			KMyMath::Vector3 point1_1 = MyEase::OutCubicVec3(start, p1, clearPhaseTimer / clearPhaseTime);
			KMyMath::Vector3 point1_2 = MyEase::OutCubicVec3(p1, end, clearPhaseTimer / clearPhaseTime);
			KMyMath::Vector3 point1 = MyEase::OutCubicVec3(point1_1, point1_2, clearPhaseTimer / clearPhaseTime);

			// ポイント２の制御点
			KMyMath::Vector3 point2_1 = MyEase::OutCubicVec3(start, p2, clearPhaseTimer / clearPhaseTime);
			KMyMath::Vector3 point2_2 = MyEase::OutCubicVec3(p2, end, clearPhaseTimer / clearPhaseTime);
			KMyMath::Vector3 point2 = MyEase::OutCubicVec3(point2_1, point2_2, clearPhaseTimer / clearPhaseTime);

			player->SetPos(MyEase::OutCubicVec3(point1, point2, clearPhaseTimer / clearPhaseTime));
			player->SetScale(MyEase::InCubicVec3({ 2.0f,2.0f,2.0f }, { 0.0f,0.0f,0.0f }, clearPhaseTimer / clearPhaseTime));
			player->SetRot(MyEase::OutCubicVec3({ 0.0f,0.0f,0.0f }, { -45.0f,-45.0f,45.0f }, clearPhaseTimer / clearPhaseTime));

			camera->SetCameraRot({ MyEase::OutCubicFloat(0.0f,-15.0f,clearPhaseTimer / clearPhaseTime) ,camera->GetCameraRot().y, camera->GetCameraRot().z });
		}
		else
		{
			clearPhaseTimer = 0;
			clearPhase++;
		}
	}
	// 暗転
	else if (clearPhase == 3)
	{
		sceneChange->SceneChangeStart();
		clearPhase++;
	}
	// リザルトシーンへ
	else
	{
		if (sceneChange->GetIsChange())
		{
			sceneManager->ChangeScene("CLEAR");
			bulletManager->AllBulletDelete();
		}
	}
}

void GameScence::MovieBarInInit()
{
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	movieBarPos[0] = { 0.0f,0.0f };
	movieBarPos[1] = { 0.0f,height };
}

void GameScence::MovieBarOutInit()
{
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	movieBarPos[0] = { 0.0f,-50.0f };
	movieBarPos[1] = { 0.0f,height + 50.0f };
}

void GameScence::MovieBarOut(const float timer_)
{
	// ムービーバーを上下へ
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	movieBarPos[0] = MyEase::Lerp2D({ 0.0f,0.0f }, { 0.0f,-50.0f }, timer_);
	movieBarPos[1] = MyEase::Lerp2D({ 0.0f,height }, { 0.0f,height + 50.0f }, timer_);
}

void GameScence::MovieBarIn(const float timer_)
{
	// ムービーバーをにょっき
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	movieBarPos[0] = MyEase::Lerp2D({ 0.0f,-50.0f }, { 0.0f,0.0f }, timer_);
	movieBarPos[1] = MyEase::Lerp2D({ 0.0f,height + 50.0f }, { 0.0f,height }, timer_);
}

const bool GameScence::GetIsStart() const
{
	return isStageStart;
}
