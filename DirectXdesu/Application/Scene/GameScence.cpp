#include "GameScence.h"
#include "KInput.h"

#include "MyMath.h"

#include "DebugCamera.h"
#include "GameCamera.h"

#include "Collision.h"
#include <iomanip>
#include <sstream>

#include "FbxLoader.h"

#include "SceneManager.h"

#include "AppearEnemy.h"
#include "CanonEnemy.h"
#include "FlyEnemy.h"

#include "Ease.h"

#include "ScoreManager.h"

#include "GameManager.h"

#include "PipelineManager.h"

#include "ModelManager.h"

GameScence::~GameScence() { Final(); };

void GameScence::LoadResources() {}

void GameScence::Init() {
	BaseScene::Init();

	// インスタンス
	input = KInput::GetInstance();

	// プレイヤー生成
	float playersHPInit = 50.0f;
	player.reset(Player::Create(playersHPInit));

	// カメラ生成
	camera = std::make_unique<RailCamera>();

	// シーンマネージャーインスタンス
	sceneManager = SceneManager::GetInstance();

	// カメラ初期化
	camera->Init(player.get(), {0.0f, 0.0f, -200.0f});
	//camera->Init(player.get(), {0.0f, 0.0f, 450.0f});

	// エネミーマネージャー生成
	enemyManager.reset(EnemyManager::Create(
	    "Resources/csv/enemyPop.csv", // ステージのcsvを読み込む
	    player.get()                  // プレイヤー情報
	    ));

	// 地面
	ground = std::make_unique<Ground>();
	ground->Init(player.get());

	// スカイボックス
	skyBox.reset(SkyBox::Create(
	    ModelManager::GetInstance()->GetModels("SkyDorm"),
	    PipelineManager::GetInstance()->GetObjPipeline(), 50));

	// 弾マネージャー
	bulletManager = BulletManager::GetInstance();
	bulletManager->Init();

	// パーティクル
	particleManager = ParticleManager::GetInstance();
	particleManager->Init();
	objParticleManager = ObjParticleManager::GetInstance();
	objParticleManager->Init();

	// ビル
	billManager = std::make_unique<BillManager>();
	billManager->Init();

	for (size_t i = 0; i < 2; i++) {
		movieBar[i].reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	}

	isCallDeadCamera = false;

	isStageStart = true;

	poseBack.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));

	selectBar.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));

	poseTexS.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));

	backTitleS.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));

	operationS.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));

	ScoreManager::GetInstance()->Init();
	ScoreManager::GetInstance()->ResetScore();
	ScoreManager::GetInstance()->SetDamageCountMax((size_t)playersHPInit);

	audioManager = AudioManager::GetInstance();

	audioManager->BGMPlay_wav("BattleBGM.wav", 0.25f);
}

void GameScence::Update() {
	if (isStageStart) {
		billManager->SetIsStopCreate(true);

		StageStartMovie();
	} else if (isBossAppearMovie) {
		BossAppearMovie();
	} else if (isClearMovie) {
		ClearMovie();
	} else if (isOverMovie) {
		GoGameOverScene();
	} else {
		GameManager::GetInstance()->SetIsStartMovie(true);

		if (input->GetPadButtonDown(XINPUT_GAMEPAD_START)) {
			if (isPose) {
				audioManager->BGMPlay_wav("BattleBGM.wav", 0.25f);
				isPose = false;
			} else {
				audioManager->SoundStopWave("BattleBGM.wav");
				isPose = true;
			}
		}

		if (!isPose) {
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
	}

	if (!isPose) {
		// ボスの更新
		if (blaster) {
			if (blaster->GetIsFallEffectEnd()) {
				goClearMovieTimer++;
				if (goClearMovieTimer == goClearMovieTime) {
					player->SetParent(nullptr);
					player->SetPos(player->GetWorldPos());
					isClearMovie = true;
					goClearMovieTimer = goClearMovieTime + 1.0f;
				}
			}

			blaster->Update(camera->GetViewPro(), isBossAppearMovie);
			Blaster::nowBlaster = blaster.get();
		}

		// プレイヤーの更新
		player->Update(camera->GetViewPro(), isStageStart, isBossAppearMovie, isClearMovie);
		Player::nowPlayer = player.get();

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

		ScoreManager::GetInstance()->Update();

		// ボス登場警告
		if (bossWarning) {
			bossWarning->Update();
		}
	} else {
		PoseAction();
	}
}

void GameScence::ObjDraw() {
	// 地面描画
	ground->Draw();

	// モブエネミー描画
	enemyManager->Draw();

	// ボス描画
	if (blaster) {
		blaster->Draw();
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
	// billManager->Draw();
}

void GameScence::SpriteDraw() {
	// ボス登場警告演出
	if (bossWarning) {
		bossWarning->Draw();
	}

	// UI描画
	player->UIDraw();

	if (blaster) {
		// ボスUI描画
		blaster->UIDraw();
	}

	// 2D情報描画
	player->SpriteDraw();

	if (isOverMovie || isClearMovie || isStageStart || isBossAppearMovie) {
		for (size_t i = 0; i < 2; i++) {
			movieBar[0]->Draw(
			    TextureManager::GetInstance()->GetTextures("White1x1"), movieBarPos[0],
			    {static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW()), 50.0f}, 0.0f,
			    {0.0f, 0.0f, 0.0f, 1.0f}, false, false, {0.0f, 0.0f});

			movieBar[1]->Draw(
			    TextureManager::GetInstance()->GetTextures("White1x1"), movieBarPos[1],
			    {static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW()), 50.0f}, 0.0f,
			    {0.0f, 0.0f, 0.0f, 1.0f}, false, false, {0.0f, 1.0f});
		}
	} else {
		ScoreManager::GetInstance()->Draw();
	}

	if (isPose) {
		float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
		float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

		poseBack->Draw(
		    TextureManager::GetInstance()->GetTextures("White1x1"), {0, 0}, {width, height}, 0.0f,
		    {0, 0, 0, 0.7f}, false, false, {0, 0});

		poseTexPos = {width / 2, height * 1 / 4};
		poseTexS->Draw(
		    TextureManager::GetInstance()->GetTextures("Pose"), poseTexPos, {1.5f, 1.5f});

		selectBar->Draw(
		    TextureManager::GetInstance()->GetTextures("White1x1"), selectBarPos, {200.0f, 34.0f},
		    0.0f, {0.5f, 0.5f, 0.5f, 0.8f});

		operationS->Draw(TextureManager::GetInstance()->GetTextures("Operation"), operationPos);
		backTitleS->Draw(TextureManager::GetInstance()->GetTextures("Back"), backTitlePos);
	}
}

void GameScence::Final() {}

void GameScence::CheckAllCollisions() {
	// 自機弾の取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets =
	    bulletManager->GetPlayerBullets();

	// 敵弾の取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bulletManager->GetEnemyBullets();

	// ボム
	const std::list<std::unique_ptr<Bom>>& boms = bulletManager->GetBoms();

	// 敵の取得
	const std::list<std::unique_ptr<MobEnemy>>& mobEnemys = enemyManager->GetMobEnemys();

	// 敵弾と自機の当たり判定
	{
		// 判定対象AとBの座標
		KMyMath::Vector3 posA, posB;

		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
			// 弾がないor自機が死んでるor自機が無敵状態の時はスキップ
			if (!bullet || player->GetIsDead()) {
				return;
			}

			if (blaster) {
				if (blaster->GetIsDead()) {
					return;
				}
			}

			// 弾の座標
			posA = bullet->GetWorldPos();

			posB = player->GetWorldPos();

			// 球同士の交差判定
			const float bulletRange = 2.0f;
			const float playerRange = 6.0f;
			if (MyCollisions::CheckSphereToSphere(posA, posB, bulletRange, playerRange)) {
				// 弾消去
				bullet->OnCollision();

				// 無敵状態じゃないとき
				if (!player->GetIsInvisible()) {
					// 自機被弾処理
					player->OnCollision(bullet->GetBulletPower());
				}
			}
		}
	}

	// ボスと自機の当たり判定
	{
		// 判定対象AとBの座標
		KMyMath::Vector3 posA, posB;

		if (blaster && !blaster->GetIsDead() && isBossBattle && !player->GetIsDead()) {
			posA = player->GetWorldPos();

			posB = blaster->GetWorldPos();

			if (MyCollisions::CheckSphereToSphere(posA, posB, 6.0f, 12.0f)) {
				player->OnCollision(10.0f);
			}
		}
	}

	// ボスユニットと自機の当たり判定
	{
		// 判定対象AとBの座標
		KMyMath::Vector3 posA;
		std::array<KMyMath::Vector3, 8> posB;

		if (blaster && !blaster->GetIsDead() && isBossBattle && !player->GetIsDead()) {
			posA = player->GetWorldPos();

			for (uint32_t i = 0; i < 8; i++) {
				posB[i] = blaster->UnitsGetWorldPos(i);

				if (MyCollisions::CheckSphereToSphere(posA, posB[i], 3.0f, 4.0f)) {
					player->OnCollision(1.0f);
				}
			}
		}
	}

	// レーザーと自機の当たり判定
	{
		// 判定対象AとBの座標
		KMyMath::Vector3 posA;
		std::array<KMyMath::Vector3, 16> posB;

		if (blaster && !blaster->GetIsDead() && isBossBattle && !player->GetIsDead()) {
			posA = player->GetWorldPos();

			for (size_t i = 0; i < 8; i++) {
				size_t j = i + 8;
				posB[i] = bulletManager->GetLazersPos(i);
				posB[j] = bulletManager->GetLazersPos(j);

				if (MyCollisions::CheckBoxToBox(
				        posA, posB[i], {3.0f, 3.0f, 3.0f}, {1.0f, 180.0f, 1.0f})) {
					player->OnCollision(5.0f);
				}

				if (MyCollisions::CheckBoxToBox(
				        posA, posB[j], {3.0f, 3.0f, 3.0f}, {180.0f, 1.0f, 1.0f})) {
					player->OnCollision(5.0f);
				}
			}
		}
	}

	// 自弾と敵の当たり判定
	{
		// 判定対象AとBの座標
		KMyMath::Vector3 posA, posB;

		for (const std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys) {
			if (!mobEnemy) {
				return;
			}

			// 敵の座標
			posA = mobEnemy->GetWorldPos();

			for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
				if (!bullet) {
					return;
				}

				// 弾の座標
				posB = bullet->GetWorldPos();

				// 球同士の交差判定
				const float enemyRange = 6.0f;
				const float bulletRange = 1.0f;
				if (MyCollisions::CheckSphereToSphere(posA, posB, enemyRange, bulletRange)) {
					// 弾消去
					bullet->OnCollision();

					// 敵消去
					mobEnemy->OnCollision();
				}
			}
		}
	}

	// 自弾とボスの判定
	{
		// 判定対象AとBの座標
		KMyMath::Vector3 posA, posB;

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			// 弾が無いorボスがいないorボスが死んでるorボスバトルが始まってなかったorプレイヤーが死んでいたらスキップ
			if (!bullet || !blaster || blaster->GetIsDead() || !isBossBattle ||
			    player->GetIsDead()) {
				return;
			}

			// 弾の座標
			posA = bullet->GetWorldPos();

			// ボスの座標
			posB = blaster->GetWorldPos();

			// 球同士の交差判定
			if (MyCollisions::CheckSphereToSphere(posA, posB, 1, 8)) {
				// 弾消去
				bullet->OnCollision();

				// 敵消去
				blaster->OnCollision(bullet->GetBulletPower());
			}
		}
	}

	// ボムと敵の当たり判定
	{
		// 判定対象AとBの座標
		KMyMath::Vector3 posA, posB;

		for (const std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys) {
			if (!mobEnemy) {
				return;
			}

			// 敵の座標
			posA = mobEnemy->GetWorldPos();

			for (const std::unique_ptr<Bom>& bom : boms) {
				if (!bom) {
					return;
				}

				// 弾の座標
				posB = bom->GetWorldPos();

				// 球同士の交差判定
				const float enemyRange = 6.0f;
				const float bulletRange = 4.0f;
				if (MyCollisions::CheckSphereToSphere(posA, posB, enemyRange, bulletRange)) {
					// 弾消去
					bom->OnCollision();

					// 敵消去
					mobEnemy->OnCollision();
				}

				if (bom->GetIsExp()) {
					const float ExpRange = 75.0f;

					if (MyCollisions::CheckSphereToSphere(posA, posB, enemyRange, ExpRange)) {
						// 敵消去
						mobEnemy->OnCollision();
					}
				}
			}
		}
	}

	// ボムとボスの当たり判定
	{
		// 判定対象AとBの座標
		KMyMath::Vector3 posA, posB;

		for (const std::unique_ptr<Bom>& bom : boms) {
			// 弾が無いorボスがいないorボスが死んでるorボスバトルが始まってなかったorプレイヤーが死んでいたらスキップ
			if (!bom || !blaster || blaster->GetIsDead() || !isBossBattle || player->GetIsDead() ||
			    bom->GetBomHit()) {
				return;
			}

			// 弾の座標
			posA = bom->GetWorldPos();

			// ボスの座標
			posB = blaster->GetWorldPos();

			// 球同士の交差判定
			if (MyCollisions::CheckSphereToSphere(posA, posB, 4, 8)) {
				// 弾消去
				bom->OnCollision();

				// ボスダメージ
				blaster->OnCollision(bom->GetBomsPower());
			}

			if (bom->GetIsExp()) {
				const float ExpRange = 75.0f;

				if (MyCollisions::CheckSphereToSphere(posA, posB, ExpRange, 8)) {
					// ボスダメージ
					blaster->OnCollision(bom->GetExpPower());

					// 多段ヒット防止
					bom->SetBomHit(true);
				}
			}
		}
	}
}

void GameScence::BossBattleStart() {
	if (isWarnning) {
		return;
	}

	// ボスバトルが始まってればスキップ
	if (isBossBattle || isBossAppearMovie) {
		return;
	}

	// ボスバトル開始座標
	bossBattleStartPos = 500;

	if (!bossWarning) {
		bool isBossBattleStart = camera->GetCameraPos().z >= bossBattleStartPos;

		// スタート位置にいなかったらスキップ
		if (!isBossBattleStart) {
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

		// 敵を全削除
		enemyManager->AllEnemyDelete();
	} else {
		// 演出が終わってないときは抜ける
		if (!bossWarning->GetIsDelete()) {
			return;
		}

		//
		sceneChange->SceneChangeStart();

		// ボス登場警告解放
		bossWarning.reset();

		// ボス出現ムービーへ
		isBossAppearMovie = true;

		isWarnning = true;
	}
}

void GameScence::PlayerDead() {
	if (player->GetIsDead() && !isCallDeadCamera) {
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

void GameScence::StageStartMovie() {
	// スキップしよう
	if (startPhase < 5) {
		if (input->GetPadButtonDown(XINPUT_GAMEPAD_START) ||
		    GameManager::GetInstance()->GetIsStartMovie()) {
			startPhase = 5;
		}
	}

	// カメラワーク一段階(上から見下ろし)
	if (startPhase == 0) {
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0) {
			MovieBarInInit();

			// 自機とカメラの距離
			const KMyMath::Vector3 playerDistance = {0.0f, 20.0f, 40.0f};

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			const float rotX = 20.0f;
			const float rotY = 180.0f;
			camera->SetCameraRot({rotX, rotY, camera->GetCameraRot().z});

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime) {
			startPhaseTimer++;

			const float startPosY = 20.0f;
			const float endPosY = 10.0f;

			const float startPosZ = 40.0f;
			const float endPosZ = 45.0f;

			camera->SetCameraPos(
			    {camera->GetCameraPos().x,
			     MyEase::Lerp(
			         player->GetWorldPos().y + startPosY, player->GetWorldPos().y + endPosY,
			         startPhaseTimer / startPhaseTime),
			     MyEase::Lerp(
			         player->GetWorldPos().z + startPosZ, player->GetWorldPos().z + endPosZ,
			         startPhaseTimer / startPhaseTime)});

			const float startRotX = 20.0f;
			const float endRotX = 10.0f;

			camera->SetCameraRot(
			    {MyEase::Lerp(startRotX, endRotX, startPhaseTimer / startPhaseTime),
			     camera->GetCameraRot().y, camera->GetCameraRot().z});
		} else {
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// カメラワーク二段階(自機右上から至近距離)
	else if (startPhase == 1) {
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0) {
			// 自機とカメラの距離
			const KMyMath::Vector3 playerDistance = {2.5f, 2.5f, 3.5f};

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			const float rotX = 45.0f;
			const float rotY = 250.0f;
			camera->SetCameraRot({rotX, rotY, camera->GetCameraRot().z});

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime) {
			startPhaseTimer++;

			const float startPosZ = 3.5f;
			const float endPosZ = 1.5f;

			camera->SetCameraPos(
			    {camera->GetCameraPos().x, camera->GetCameraPos().y,
			     MyEase::Lerp(
			         player->GetWorldPos().z + startPosZ, player->GetWorldPos().z + endPosZ,
			         startPhaseTimer / startPhaseTime)});
		} else {
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// カメラワーク三段階(自機左したからブースター(ケツ)注視)
	else if (startPhase == 2) {
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0) {
			// 自機とカメラの距離
			const KMyMath::Vector3 playerDistance = {-2.0f, -1.8f, -3.5f};

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			const float rotX = -35.0f;
			const float rotY = 27.5f;
			camera->SetCameraRot({rotX, rotY, camera->GetCameraRot().z});

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime) {
			startPhaseTimer++;

			const float startPosX = 2.0f;
			const float endPosX = 1.5f;
			camera->SetCameraPos(
			    {MyEase::Lerp(
			         player->GetWorldPos().x - startPosX, player->GetWorldPos().x - endPosX,
			         startPhaseTimer / startPhaseTime),
			     camera->GetCameraPos().y, camera->GetCameraPos().z});

			const float startRotY = 35.0f;
			const float endRotY = 27.5f;
			camera->SetCameraRot(
			    {camera->GetCameraRot().x,
			     MyEase::Lerp(startRotY, endRotY, startPhaseTimer / startPhaseTime),
			     camera->GetCameraRot().z});
		} else {
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// カメラワーク四段階(正面でカメラを引く)
	else if (startPhase == 3) {
		startPhaseTime = 120.0f;

		if (startPhaseTimer == 0) {
			// 自機とカメラの距離
			const KMyMath::Vector3 playerDistance = {0.0f, 1.0f, 10.0f};

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			const float rotX = 0.0f;
			const float rotY = 180.0f;
			camera->SetCameraRot({rotX, rotY, camera->GetCameraRot().z});

			// カメラ動け
			camera->SetCameraPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime) {
			startPhaseTimer++;

			// ムービーバーを上下へ
			MovieBarOut(startPhaseTimer / startPhaseTime);

			const float startPosZ = 10.0f;
			const float endPosZ = 30.0f;

			camera->SetCameraPos(
			    {camera->GetCameraPos().x, camera->GetCameraPos().y,
			     MyEase::OutCubicFloat(
			         player->GetWorldPos().z + startPosZ, player->GetWorldPos().z + endPosZ,
			         startPhaseTimer / startPhaseTime)});
		} else {
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// 自機が向かってくる
	else if (startPhase == 4) {
		startPhaseTime = 60;

		if (startPhaseTimer < startPhaseTime) {
			startPhaseTimer++;

			const float startPosZ = 50.0f;
			const float endPosZ = 100.0f;

			player->SetPos(
			    {player->GetPosition().x, player->GetPosition().y,
			     MyEase::OutCubicFloat(startPosZ, endPosZ, startPhaseTimer / startPhaseTime)});
		} else {
			startPhase++;
			startPhaseTimer = 0;
		}
	} else {
		MovieBarOutInit();
		billManager->SetIsStopCreate(false);
		camera->EndStart();
		player->EndStart();
		Player::isStartEase = true;
		// 親子関係接続
		player->SetParent(&camera->GetTransform());
		// bgmManager->SoundPlay(bgmManager->GetBGM1());
		isStageStart = false;
	}
}

void GameScence::GoGameOverScene() {
	if (player->GetIsFallEffectEnd()) {
		isGoOverScene = true;
	}

	if (isGoOverScene) {
		goOverSceneTimer++;
		if (goOverSceneTimer == goOverSceneTime) {
			sceneChange->SceneChangeStart();
			audioManager->SoundStopWave("BattleBGM.wav");
			goOverSceneTimer = goOverSceneTime + 1.0f;
		}

		if (sceneChange->GetIsChange()) {
			sceneManager->ChangeScene("GAME");
			bulletManager->AllBulletDelete();
		}
	}
}

void GameScence::BossAppearMovie() {
	// スキップしよう
	if (appearPhase > 0 && appearPhase < 7) {
		if (input->GetPadButtonDown(XINPUT_GAMEPAD_START)) {
			appearPhaseTimer = 0;
			appearPhase = 7;
		}
	}

	// 暗転待ち時間
	if (appearPhase == 0) {
		appearPhaseTime = 30;

		if (appearPhaseTimer < appearPhaseTime) {
			appearPhaseTimer++;
		} else {
			// ボス配置
			const float bossDistance = 150;
			const KMyMath::Vector3 bossBasePos = {0.0f, 120.0f, bossBattleStartPos + bossDistance};

			// 生成
			blaster.reset(Blaster::Create(
			    PipelineManager::GetInstance()->GetObjPipeline(), bossBasePos,
			    PipelineManager::GetInstance()->GetSpritePipeline()));

			// すべての弾削除
			bulletManager->AllBulletDelete();
			// プレイヤーとカメラの親子関係解消
			player->SetParent(nullptr);
			// 現在位置まで連れてくる
			player->SetPos({0.0f, 0.0f, player->GetWorldPos().z});
			// 回転角度初期化
			player->SetRot({0.0f, 0.0f, 0.0f});
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ムービーフェーズ1
	else if (appearPhase == 1) {
		appearPhaseTime = 90.0f;

		MovieBarInInit();

		if (appearPhaseTimer < appearPhaseTime) {
			appearPhaseTimer++;

			// ボス降下
			const float startBPosY = 40.0f;
			const float endBPosY = 20.0f;
			blaster->SetPos(
			    {blaster->GetWorldPos().x,
			     MyEase::OutCubicFloat(startBPosY, endBPosY, appearPhaseTimer / appearPhaseTime),
			     blaster->GetWorldPos().z});

			// カメラの最終位置
			KMyMath::Vector3 cameraMove = {0.0f, 0.0f, -30.0f};

			// カメラの場所
			const KMyMath::Vector3 cameraPos = {
			    blaster->GetWorldPos().x + cameraMove.x, endBPosY + cameraMove.y,
			    blaster->GetWorldPos().z + cameraMove.z};

			camera->SetCameraPos(cameraPos);

			const KMyMath::Vector3 rot = {0.0f, 0.0f, 0.0f};
			camera->SetCameraRot(rot);
		} else {
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ムービーフェーズ2
	else if (appearPhase == 2) {
		appearPhaseTime = 30.0f;

		if (appearPhaseTimer < appearPhaseTime) {
			appearPhaseTimer++;

			// 自機とカメラの距離
			KMyMath::Vector3 cameraMove = {
			    0.0f, 0.0f,
			    MyEase::OutCubicFloat(-30.0f, -20.0f, appearPhaseTimer / appearPhaseTime)};

			// カメラの場所
			const KMyMath::Vector3 cameraPos = {
			    blaster->GetWorldPos().x + cameraMove.x, blaster->GetWorldPos().y + cameraMove.y,
			    blaster->GetWorldPos().z + cameraMove.z};

			camera->SetCameraPos(cameraPos);

			const KMyMath::Vector3 rot = {0.0f, 0.0f, 0.0f};
			camera->SetCameraRot(rot);
		} else {
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// 小休止
	else if (appearPhase == 3) {
		appearPhaseTime = 30.0f;

		if (appearPhaseTimer < appearPhaseTime) {
			appearPhaseTimer++;
		} else {
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ムービーフェーズ4
	else if (appearPhase == 4) {
		appearPhaseTime = 10.0f;

		if (appearPhaseTimer < appearPhaseTime) {
			appearPhaseTimer++;

			const float startNum = 3.0f;
			const float endNum = 4.0f;

			std::array<KMyMath::Vector3, 8> unitMove;
			unitMove[0] = MyEase::Lerp3D(
			    {startNum, startNum, startNum}, {endNum, endNum, endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[1] = MyEase::Lerp3D(
			    {-startNum, startNum, startNum}, {-endNum, endNum, endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[2] = MyEase::Lerp3D(
			    {startNum, startNum, -startNum}, {endNum, endNum, -endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[3] = MyEase::Lerp3D(
			    {-startNum, startNum, -startNum}, {-endNum, endNum, -endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[4] = MyEase::Lerp3D(
			    {startNum, -startNum, startNum}, {endNum, -endNum, endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[5] = MyEase::Lerp3D(
			    {-startNum, -startNum, startNum}, {-endNum, -endNum, endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[6] = MyEase::Lerp3D(
			    {startNum, -startNum, -startNum}, {endNum, -endNum, -endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[7] = MyEase::Lerp3D(
			    {-startNum, -startNum, -startNum}, {-endNum, -endNum, -endNum},
			    appearPhaseTimer / appearPhaseTime);

			for (size_t i = 0; i < 8; i++) {
				blaster->SetUnitsPos(unitMove[i], i);
			}
		} else {
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// 小休止
	else if (appearPhase == 5) {
		appearPhaseTime = 30.0f;

		if (appearPhaseTimer < appearPhaseTime) {
			appearPhaseTimer++;
		} else {
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ムービーフェーズ6
	else if (appearPhase == 6) {
		appearPhaseTime = 180.0f;

		if (appearPhaseTimer < appearPhaseTime) {
			if (appearPhaseTimer == 0) {
				blaster->SetFarstAct();
			}

			appearPhaseTimer++;

			const float startNum = 4.0f;
			const float endNum = 6.0f;

			std::array<KMyMath::Vector3, 8> unitMove;
			unitMove[0] = MyEase::Lerp3D(
			    {startNum, startNum, startNum}, {endNum, endNum, endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[1] = MyEase::Lerp3D(
			    {-startNum, startNum, startNum}, {-endNum, endNum, endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[2] = MyEase::Lerp3D(
			    {startNum, startNum, -startNum}, {endNum, endNum, -endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[3] = MyEase::Lerp3D(
			    {-startNum, startNum, -startNum}, {-endNum, endNum, -endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[4] = MyEase::Lerp3D(
			    {startNum, -startNum, startNum}, {endNum, -endNum, endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[5] = MyEase::Lerp3D(
			    {-startNum, -startNum, startNum}, {-endNum, -endNum, endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[6] = MyEase::Lerp3D(
			    {startNum, -startNum, -startNum}, {endNum, -endNum, -endNum},
			    appearPhaseTimer / appearPhaseTime);
			unitMove[7] = MyEase::Lerp3D(
			    {-startNum, -startNum, -startNum}, {-endNum, -endNum, -endNum},
			    appearPhaseTimer / appearPhaseTime);

			for (size_t i = 0; i < 8; i++) {
				blaster->SetUnitsPos(unitMove[i], i);
			}

			// カメラの動き
			KMyMath::Vector3 cameraMove = MyEase::InOutCubicVec3(
			    {0.0f, 0.0f, -20.0f}, {10.0f, -20.0f, -120.0f}, appearPhaseTimer / appearPhaseTime);

			// カメラの場所
			const KMyMath::Vector3 cameraPos = blaster->GetWorldPos() + cameraMove;
			camera->SetCameraPos(cameraPos);

			const KMyMath::Vector3 rot = {
			    0.0f, MyEase::InOutCubicFloat(0.0f, -15.0f, appearPhaseTimer / appearPhaseTime),
			    MyEase::InOutCubicFloat(0.0f, 15.0f, appearPhaseTimer / appearPhaseTime)};
			camera->SetCameraRot(rot);
		} else {
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// 暗転
	else if (appearPhase == 7) {
		appearPhaseTime = 30;

		if (appearPhaseTimer == 0) {
			sceneChange->SceneChangeStart();
		}

		if (appearPhaseTimer < appearPhaseTime) {
			appearPhaseTimer++;
		} else {
			appearPhaseTimer = 0;
			appearPhase++;
		}
	}
	// ボス戦配置
	else if (appearPhase == 8) {
		blaster->SetFarstAct();
		MovieBarOutInit();
		// ボス配置
		blaster->SetPos({blaster->GetWorldPos().x, 20.0f, blaster->GetWorldPos().z});
		blaster->SetRot({0.0f, 0.0f, 0.0f});
		// カメラ配置
		camera->SetCameraPos({0.0f, 0.0f, bossBattleStartPos});
		camera->SetCameraRot({0.0f, 0.0f, 0.0f});
		// プレイヤーとカメラの親子関係解消
		player->SetParent(&camera->GetTransform());
		// 現在位置まで連れてくる
		player->SetPos({0.0f, 0.0f, 50.0f});
		appearPhaseTimer = 0;
		appearPhase++;
	} else {
		// ムービー終わり
		isBossAppearMovie = false;
		// ボスバトル開始
		isBossBattle = true;
	}
}

void GameScence::BossBreakMovie() {}

void GameScence::ClearMovie() {
	// スキップ
	if (clearPhase < 3) {
		if (input->GetPadButtonDown(XINPUT_GAMEPAD_START)) {
			clearPhaseTimer = 0;
			clearPhase = 3;
		}
	}

	// フェーズ0
	if (clearPhase == 0) {
		clearPhaseTime = 30.0f;

		if (clearPhaseTimer < clearPhaseTime) {
			if (clearPhaseTimer == 0) {
				audioManager->SoundStopWave("BattleBGM.wav");
				audioManager->SEPlay_wav("mokuhyo.wav");
			}

			clearPhaseTimer++;

			MovieBarIn(clearPhaseTimer / clearPhaseTime);

			player->SetPos(MyEase::InCubicVec3(
			    player->GetWorldPos(), {0.0f, 0.0f, player->GetWorldPos().z},
			    clearPhaseTimer / clearPhaseTime));
			player->SetRot(MyEase::InCubicVec3(
			    player->GetRot(), {0.0f, 0.0f, 0.0f}, clearPhaseTimer / clearPhaseTime));

			const KMyMath::Vector3 dhistans = {0.0f, 0.0f, -40.0f};

			const KMyMath::Vector3 playerPos = {0.0f, 0.0f, player->GetWorldPos().z};

			const KMyMath::Vector3 cameraPos = playerPos + dhistans;

			camera->SetCameraPos(MyEase::InOutCubicVec3(
			    camera->GetCameraPos(), cameraPos, clearPhaseTimer / clearPhaseTime));

			camera->SetCameraRot({0.0f, 0.0f, 0.0f});
		} else {
			clearPhaseTimer = 0;
			clearPhase++;
		}
	}
	// フェーズ1
	else if (clearPhase == 1) {
		clearPhaseTime = 180.0f;

		if (clearPhaseTimer < clearPhaseTime) {
			clearPhaseTimer++;

			// 角度を変更
			float rotAngle =
			    MyEase::InOutCubicFloat(0.0f, -60.0f, clearPhaseTimer / clearPhaseTime);

			const float radian = DirectX::XMConvertToRadians(rotAngle);
			const float distance = -30;

			const KMyMath::Vector3 cameraPos = {
			    (distance * sinf(radian)), 0.0f, (distance * cosf(radian))};
			camera->SetCameraPos(player->GetWorldPos() + cameraPos);

			camera->SetCameraRot({0.0f, rotAngle, 0.0f});
		} else {
			clearPhaseTimer = 0;
			clearPhase++;
		}
	}
	// フェーズ2
	else if (clearPhase == 2) {
		if (clearPhaseTimer == 0) {
			start = {0.0f, 0.0f, player->GetWorldPos().z};
			p1 = {-20.0f, 0.0f, player->GetWorldPos().z + 75.0f};
			p2 = {-50.0f, 75.0f, player->GetWorldPos().z + 100.0f};
			end = {-100.0f, 100.0f, player->GetWorldPos().z + 125.0f};
		}

		clearPhaseTime = 210.0f;

		if (clearPhaseTimer < clearPhaseTime) {
			clearPhaseTimer++;

			// ポイント１の制御点
			KMyMath::Vector3 point1_1 =
			    MyEase::OutCubicVec3(start, p1, clearPhaseTimer / clearPhaseTime);
			KMyMath::Vector3 point1_2 =
			    MyEase::OutCubicVec3(p1, end, clearPhaseTimer / clearPhaseTime);
			KMyMath::Vector3 point1 =
			    MyEase::OutCubicVec3(point1_1, point1_2, clearPhaseTimer / clearPhaseTime);

			// ポイント２の制御点
			KMyMath::Vector3 point2_1 =
			    MyEase::OutCubicVec3(start, p2, clearPhaseTimer / clearPhaseTime);
			KMyMath::Vector3 point2_2 =
			    MyEase::OutCubicVec3(p2, end, clearPhaseTimer / clearPhaseTime);
			KMyMath::Vector3 point2 =
			    MyEase::OutCubicVec3(point2_1, point2_2, clearPhaseTimer / clearPhaseTime);

			player->SetPos(MyEase::OutCubicVec3(point1, point2, clearPhaseTimer / clearPhaseTime));
			player->SetScale(MyEase::InCubicVec3(
			    {2.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, clearPhaseTimer / clearPhaseTime));
			player->SetRot(MyEase::OutCubicVec3(
			    {0.0f, 0.0f, 0.0f}, {-45.0f, -45.0f, 45.0f}, clearPhaseTimer / clearPhaseTime));

			camera->SetCameraRot(
			    {MyEase::OutCubicFloat(0.0f, -15.0f, clearPhaseTimer / clearPhaseTime),
			     camera->GetCameraRot().y, camera->GetCameraRot().z});
		} else {
			clearPhaseTimer = 0;
			clearPhase++;
		}
	}
	// 暗転
	else if (clearPhase == 3) {
		sceneChange->SceneChangeStart();
		GameManager::GetInstance()->SetIsStartMovie(false);
		clearPhase++;
	}
	// リザルトシーンへ
	else {
		if (sceneChange->GetIsChange()) {
			sceneManager->ChangeScene("CLEAR");
			bulletManager->AllBulletDelete();
		}
	}
}

void GameScence::MovieBarInInit() {
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	movieBarPos[0] = {0.0f, 0.0f};
	movieBarPos[1] = {0.0f, height};
}

void GameScence::MovieBarOutInit() {
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	movieBarPos[0] = {0.0f, -50.0f};
	movieBarPos[1] = {0.0f, height + 50.0f};
}

void GameScence::MovieBarOut(const float timer_) {
	// ムービーバーを上下へ
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	movieBarPos[0] = MyEase::Lerp2D({0.0f, 0.0f}, {0.0f, -50.0f}, timer_);
	movieBarPos[1] = MyEase::Lerp2D({0.0f, height}, {0.0f, height + 50.0f}, timer_);
}

void GameScence::MovieBarIn(const float timer_) {
	// ムービーバーをにょっき
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	movieBarPos[0] = MyEase::Lerp2D({0.0f, -50.0f}, {0.0f, 0.0f}, timer_);
	movieBarPos[1] = MyEase::Lerp2D({0.0f, height + 50.0f}, {0.0f, height}, timer_);
}

void GameScence::PoseAction() {

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	operationPos = {width / 2, (height / 2) - 60.0f};
	backTitlePos = {width / 2, height / 2};

	if (input->GetLStickDown()) {
		if (isOperation) {
			isOperation = false;
			isBackTitle = true;
		}
	}

	if (input->GetLStickUp()) {
		if (isBackTitle) {
			isBackTitle = false;
			isOperation = true;
		}
	}

	if (isOperation) {
		selectBarPos = operationPos;

		if (input->GetPadButton(XINPUT_GAMEPAD_A)) {
		}
	}

	if (isBackTitle) {
		selectBarPos = backTitlePos;

		if (input->GetPadButton(XINPUT_GAMEPAD_A)) {
			sceneChange->SceneChangeStart();
		}
	}

	if (sceneChange->GetIsChange()) {
		GameManager::GetInstance()->SetIsStartMovie(false);
		sceneManager->ChangeScene("TITLE");
		bulletManager->AllBulletDelete();
	}
}

const bool GameScence::GetIsStart() const { return isStageStart; }
