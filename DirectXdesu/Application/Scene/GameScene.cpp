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
	//camera->Init(player.get(), {0.0f, 0.0f, -200.0f});
	camera->Init(player.get(), {0.0f, 0.0f, 750.0f});

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
	    PipelineManager::GetInstance()->GetPipeline("Obj"), 50));

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

	isCallDeadCamera = false;

	GameManager::GetInstance()->SetMovieFlag(true, "Start");

	poseBack.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	selectBar.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	poseTexS.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	backTitleS.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	operationS.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	// ボスバトル開始座標
	bossBattleStartPos = 900;

	ScoreManager::GetInstance()->Init();
	ScoreManager::GetInstance()->ResetScore();
	ScoreManager::GetInstance()->SetDamageCountMax((size_t)playersHPInit);

	audioManager_ = AudioManager::GetInstance();

	audioManager_->BGMPlay_wav("BattleBGM.wav", 0.15f);

	gameManager_ = GameManager::GetInstance();

	movie_ = std::make_unique<StageStart>();

	collisionManager_ = new CollisionManager();
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
		// プレイヤーとカメラの親子関係解消
		player->SetParent(&camera->GetTransform());
		GamePlay();
		break;
	case GameScene::Over:
		GoGameOverScene();
		break;
	case GameScene::Movies:
		// プレイヤーとカメラの親子関係解消
		player->SetParent(nullptr);
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
	switch (scene) {
	case GameScene::Games:
		// ボス登場警告演出
		if (bossWarning) {
			bossWarning->Draw();
		}

		if (blaster) {
			// ボスUI描画
			blaster->UIDraw();
		}

		// 2D情報描画
		player->SpriteDraw();

		// UI描画
		player->UIDraw();

		// スコア描画
		ScoreManager::GetInstance()->Draw();
		break;
	case GameScene::Over:
		// UI描画
		player->UIDraw();

		// スコア描画
		ScoreManager::GetInstance()->Draw();
		break;
	case GameScene::Movies:
		movie_->Draw();
		break;
	default:
		break;
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

void GameScene::Final() { delete collisionManager_; }

void GameScene::CheckAllCollisions() {
	// 登録当たり判定を削除
	collisionManager_->Reset();

#pragma region 準備処理
	// 自機弾の取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets =
	    bulletManager->GetPlayerBullets();

	// 敵弾の取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bulletManager->GetEnemyBullets();

	// ボム
	const std::list<std::unique_ptr<Bom>>& boms = bulletManager->GetBoms();

	// 爆発
	const std::list<std::unique_ptr<Explosion>>& explosions = bulletManager->GetExplosion();

	// 敵の取得
	const std::list<std::unique_ptr<MobEnemy>>& mobEnemys = enemyManager->GetMobEnemys();

	// 当たり判定を登録
	// 自機
	player->SetRadius(3.0f);
	collisionManager_->AddCollider(player.get());

	// 雑魚敵
	for (const std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys) {
		mobEnemy->SetRadius(6.0f);
		collisionManager_->AddCollider(mobEnemy.get());
	}

	// ボス
	if (blaster) {
		blaster->SetRadius(12.0f);
		collisionManager_->AddCollider(blaster.get());
	}

	// 自弾
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets) {
		collisionManager_->AddCollider(playerBullet.get());
	}

	// ボム
	for (const std::unique_ptr<Bom>& bom : boms) {
		bom->SetRadius(5.0f);
		collisionManager_->AddCollider(bom.get());
	}

	for (const std::unique_ptr<Explosion>& explosion : explosions) {
		explosion->SetRadius(32.5f);
		collisionManager_->AddCollider(explosion.get());
	}

	// 敵弾
	for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets) {
		collisionManager_->AddCollider(enemyBullet.get());
	}
#pragma endregion

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
					player->OnCollision(player.get());
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
					player->OnCollision(player.get());
				}

				if (MyCollisions::CheckBoxToBox(
				        posA, posB[j], {3.0f, 3.0f, 3.0f}, {180.0f, 1.0f, 1.0f})) {
					player->OnCollision(player.get());
				}
			}
		}
	}

	// 総当たり判定(球と球)
	collisionManager_->CheckAllCollisions();
}

void GameScene::BossBattleStart() {
	if (isWarnning) {
		return;
	}

	// ボスバトルが始まってればスキップ
	if (isBossBattle || GameManager::GetInstance()->GetMovieFlag("Boss")) {
		return;
	}

	if (!bossWarning) {
		bool isBossBattleStart = camera->GetCameraPos().z >= bossBattleStartPos;

		// スタート位置にいなかったらスキップ
		if (!isBossBattleStart) {
			return;
		}

		// ステージBGM停止
		audioManager_->SoundStopWave("BattleBGM.wav");

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
		    PipelineManager::GetInstance()->GetPipeline("Obj"), bossBasePos,
		    PipelineManager::GetInstance()->GetPipeline("Sprite")));

		Blaster::nowBlaster = blaster.get();

		// ボス出現ムービーへ
		GameManager::GetInstance()->SetMovieFlag(true, "Boss");
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
		GameManager::GetInstance()->SetMovieFlag(true, "Over");
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
		audioManager_->SoundStopWave("BattleBGM.wav");
		audioManager_->SoundStopWave("bossBGM.wav");
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

		if (!blaster) {
			// エネミーマネージャーの更新
			enemyManager->Update(camera->GetViewPro(), camera->GetWorldPos());
		}

		// 天箱を自機に追従
		skyBox->SetPosZ(player->GetWorldPos().z);
	}
}

void GameScene::AllScene() {
	if (!isPose) {
		// ムービーが終わったらゲームシーンへ
		if (movie_->GetIsFinish()) {
			if (gameManager_->GetMovieFlag("Start")) {
				gameManager_->SetMovieFlag(false, "Start");
			}

			if (gameManager_->GetMovieFlag("Boss")) {
				gameManager_->SetMovieFlag(false, "Boss");
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
					gameManager_->SetMovieFlag(true, "Clear");
					movie_ = std::make_unique<StageClear>();
					scene = Movies;
					goClearMovieTimer = goClearMovieTime + 1.0f;
				}
			}

			blaster->Update(
			    camera->GetViewPro(), camera->GetWorldPos(), gameManager_->GetMovieFlag("Boss"));
			Blaster::nowBlaster = blaster.get();
		}

		// プレイヤーの更新
		player->Update(camera->GetViewPro(), camera->GetWorldPos());
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
		camera->Update(
		    gameManager_->GetMovieFlag("Start"), gameManager_->GetMovieFlag("Boss"),
		    gameManager_->GetMovieFlag("Clear"));
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

	gameManager_->Update();
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
			audioManager_->SoundStopWave("BattleBGM.wav");
			audioManager_->SoundStopWave("bossBGM.wav");
			sceneChange->SceneChangeStart();
		}
	}

	if (sceneChange->GetIsChange()) {
		gameManager_->SetIsStartMovie(false);
		sceneManager->ChangeScene("TITLE");
		bulletManager->AllBulletDelete();
	}
}
