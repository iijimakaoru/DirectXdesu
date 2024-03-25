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

#include "AppearEnemy.h"
#include "CanonEnemy.h"
#include "FlyEnemy.h"

#include "Ease.h"

#include "ScoreManager.h"

#include "PipelineManager.h"

#include "ModelManager.h"

#include "BossStart.h"
#include "StageClear.h"
#include "StageStart.h"

GameScene::~GameScene() { Final(); };

void GameScene::LoadResources() {}

void GameScene::Init() {
	BaseScene::Init();

	// インスタンス
	input = KInput::GetInstance();

	light_.reset(Light::Create());
	light_->SetLightRGB({1, 1, 1});
	KObject3d::SetLight(light_.get());

	// プレイヤー生成
	float playersHPInit = 50.0f;
	player.reset(Player::Create(playersHPInit));
	Player::nowPlayer = player.get();

	// カメラ生成
	camera = std::make_unique<RailCamera>();
	RailCamera::nowRailCamera = camera.get();

	// シーンマネージャーインスタンス
	sceneManager = SceneManager::GetInstance();

	// カメラ初期化
	// camera->Init(player.get(), {0.0f, 0.0f, -200.0f});
	camera->Init(player.get(), {0.0f, 0.0f, 470.0f});

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
	bulletManager->Init(light_.get());

	// パーティクル
	particleManager = ParticleManager::GetInstance();
	particleManager->Init();
	objParticleManager = ObjParticleManager::GetInstance();
	objParticleManager->Init(light_.get());

	// ビル
	billManager = std::make_unique<BillManager>();
	billManager->Init(light_.get());

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

	// ボスバトル開始座標
	bossBattleStartPos = 500;

	ScoreManager::GetInstance()->Init();
	ScoreManager::GetInstance()->ResetScore();
	ScoreManager::GetInstance()->SetDamageCountMax((size_t)playersHPInit);

	audioManager = AudioManager::GetInstance();

	audioManager->BGMPlay_wav("BattleBGM.wav", 0.15f);

	gameManager_ = GameManager::GetInstance();

	movie_ = std::make_unique<StageStart>();
}

void GameScene::Update() {
	ImGui::Begin("Light");
	ImGui::SetWindowPos({0, 300});
	ImGui::SetWindowSize({200, 200});
	ImGui::SliderFloat("LightColorR", &lightRGB.x, 0, 1, "%.1f");
	ImGui::SliderFloat("LightColorG", &lightRGB.y, 0, 1, "%.1f");
	ImGui::SliderFloat("LightColorB", &lightRGB.z, 0, 1, "%.1f");
	ImGui::SliderFloat("LightDirX", &lightDir.x, -1, 1, "%.1f");
	ImGui::SliderFloat("LightDirY", &lightDir.y, -1, 1, "%.1f");
	ImGui::SliderFloat("LightDirZ", &lightDir.z, -1, 1, "%.1f");
	ImGui::End();

	light_->SetLightRGB({lightRGB.x, lightRGB.y, lightRGB.z});
	light_->SetLightDir({lightDir.x, lightDir.y, lightDir.z, 0.0f});

	switch (scene) {
	case GameScene::Games:
		GamePlay();
		break;
	case GameScene::Over:
		GoGameOverScene();
		break;
	case GameScene::Movies:
		movie_->Update();
		break;
	default:
		break;
	}

	AllScene();
}

void GameScene::ObjDraw() {
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

void GameScene::SpriteDraw() {
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

void GameScene::Final() {}

void GameScene::CheckAllCollisions() {
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

void GameScene::BossBattleStart() {
	if (isWarnning) {
		return;
	}

	// ボスバトルが始まってればスキップ
	if (isBossBattle || isBossAppearMovie) {
		return;
	}

	if (!bossWarning) {
		bool isBossBattleStart = camera->GetCameraPos().z >= bossBattleStartPos;

		// スタート位置にいなかったらスキップ
		if (!isBossBattleStart) {
			return;
		}

		// ステージBGM停止
		audioManager->SoundStopWave("BattleBGM.wav");

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

		// ボス配置
		const float bossDistance = 150;
		const KMyMath::Vector3 bossBasePos = {0.0f, 120.0f, bossBattleStartPos + bossDistance};

		// 生成
		blaster.reset(Blaster::Create(
		    PipelineManager::GetInstance()->GetObjPipeline(), bossBasePos,
		    PipelineManager::GetInstance()->GetSpritePipeline()));

		Blaster::nowBlaster = blaster.get();

		// ボス出現ムービーへ
		isBossAppearMovie = true;
		movie_ = std::make_unique<BossStart>();
		scene = Movies;

		isWarnning = true;
	}
}

void GameScene::PlayerDead() {
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

	if (player->GetIsFallEffectEnd()) {
		scene = Over;
		isGoOverScene = true;
	}
}

void GameScene::GoGameOverScene() {
	goOverSceneTimer++;
	if (goOverSceneTimer == goOverSceneTime) {
		sceneChange->SceneChangeStart();
		audioManager->SoundStopWave("BattleBGM.wav");
		audioManager->SoundStopWave("bossBGM.wav");
		goOverSceneTimer = goOverSceneTime + 1.0f;
	}

	if (sceneChange->GetIsChange()) {
		sceneManager->ChangeScene("GAME");
		bulletManager->AllBulletDelete();
	}
}

void GameScene::BossBreakMovie() {}

void GameScene::GamePlay() {
	gameManager_->SetIsStartMovie(true);

	if (input->GetPadButtonDown(XINPUT_GAMEPAD_START)) {
		if (isPose) {
			isPose = false;
		} else {
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
		enemyManager->Update(camera->GetViewPro(), camera->GetWorldPos());

		// 天箱を自機に追従
		skyBox->SetPosZ(player->GetWorldPos().z);
	}
}

void GameScene::AllScene() {
	if (!isPose) {
		// ムービーが終わったらゲームシーンへ
		if (movie_->GetIsFinish()) {
			if (isStageStart) {
				isStageStart = false;
			}

			if (isBossAppearMovie) {
				isBossAppearMovie = false;
				// ボスバトル開始
				isBossBattle = true;
			}

			scene = Games;
			movie_->SetIsFinish(false);
		}

		// ボスの更新
		if (blaster) {
			if (blaster->GetIsFallEffectEnd()) {
				goClearMovieTimer++;
				if (goClearMovieTimer == goClearMovieTime) {
					player->SetParent(nullptr);
					player->SetPos(player->GetWorldPos());
					isClearMovie = true;
					movie_ = std::make_unique<StageClear>();
					scene = Movies;
					goClearMovieTimer = goClearMovieTime + 1.0f;
				}
			}

			blaster->Update(camera->GetViewPro(), camera->GetWorldPos(), isBossAppearMovie);
			Blaster::nowBlaster = blaster.get();
		}

		// プレイヤーの更新
		player->Update(
		    camera->GetViewPro(), camera->GetWorldPos(), isStageStart, isBossAppearMovie,
		    isClearMovie);
		Player::nowPlayer = player.get();

		// 弾の更新
		bulletManager->Update(camera->GetViewPro(), camera->GetWorldPos());

		// 地面の更新
		ground->Update(camera->GetViewPro(), camera->GetWorldPos());

		// スカイボックスの更新
		skyBox->Update(camera->GetViewPro(), camera->GetWorldPos());

		// パーティクルマネージャーの更新
		particleManager->Update(camera->GetViewPro());
		objParticleManager->Update(camera->GetViewPro(), camera->GetWorldPos());

		// ビルマネージャー
		billManager->Update(
		    camera->GetViewPro(), camera->GetWorldPos(), camera->GetCameraPos().z - 20.0f);

		// カメラの更新
		camera->Update(isStageStart, isBossAppearMovie, isClearMovie);
		RailCamera::nowRailCamera = camera.get();

		ScoreManager::GetInstance()->Update();

		light_->Update();

		// ボス登場警告
		if (bossWarning) {
			bossWarning->Update();
		}
	} else {
		PoseAction();
	}
}

void GameScene::PoseAction() {

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
			audioManager->SoundStopWave("BattleBGM.wav");
			audioManager->SoundStopWave("bossBGM.wav");
			sceneChange->SceneChangeStart();
		}
	}

	if (sceneChange->GetIsChange()) {
		gameManager_->SetIsStartMovie(false);
		sceneManager->ChangeScene("TITLE");
		bulletManager->AllBulletDelete();
	}
}

const bool GameScene::GetIsStart() const { return isStageStart; }
