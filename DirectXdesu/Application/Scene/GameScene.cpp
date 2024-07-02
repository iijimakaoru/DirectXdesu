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
	player_.reset(Player::Create(playersHPInit));
	Player::nowPlayer = player_.get();

	// カメラ生成
	camera_ = std::make_unique<RailCamera>();
	RailCamera::nowRailCamera = camera_.get();

	// シーンマネージャーインスタンス
	sceneManager = SceneManager::GetInstance();

	// カメラ初期化
	camera_->Init(player_.get(), {0.0f, 0.0f, -200.0f});
	//camera_->Init(player_.get(), {0.0f, 0.0f, 750.0f});

	// エネミーマネージャー生成
	enemyManager.reset(EnemyManager::Create(
	    "Resources/csv/enemyPop.csv", // ステージのcsvを読み込む
	    player_.get()                  // プレイヤー情報
	    ));

	// 地面
	ground_ = std::make_unique<Ground>();
	ground_->Init(player_.get());

	// スカイボックス
	skyBox_.reset(SkyBox::Create(
	    ModelManager::GetInstance()->GetModels("SkyDorm"),
	    PipelineManager::GetInstance()->GetPipeline("Obj"), 50));

	// 弾マネージャー
	bulletManager_ = BulletManager::GetInstance();
	bulletManager_->Init(light_.get());

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

	poseBack_.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	selectBar_.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	poseTexS_.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	backTitleS_.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	operationS_.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

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
	ImGui::SliderFloat("LightColorR", &lightRGB_.x, 0, 1, "%.1f");
	ImGui::SliderFloat("LightColorG", &lightRGB_.y, 0, 1, "%.1f");
	ImGui::SliderFloat("LightColorB", &lightRGB_.z, 0, 1, "%.1f");
	ImGui::SliderFloat("LightDirX", &lightDir_.x, -1, 1, "%.1f");
	ImGui::SliderFloat("LightDirY", &lightDir_.y, -1, 1, "%.1f");
	ImGui::SliderFloat("LightDirZ", &lightDir_.z, -1, 1, "%.1f");
	ImGui::End();

	light_->SetLightRGB({lightRGB_.x, lightRGB_.y, lightRGB_.z});
	light_->SetLightDir({lightDir_.x, lightDir_.y, lightDir_.z, 0.0f});

	switch (scene_) {
	case GameScene::Games:
		player_->SetParent(&camera_->GetTransform());
		GamePlay();
		break;
	case GameScene::Over:
		GameOverMovie();
		break;
	case GameScene::Movies:
		// プレイヤーとカメラの親子関係解消
		player_->SetParent(nullptr);
		movie_->Update();
		break;
	default:
		break;
	}

	AllScene();
}

void GameScene::ObjDraw() {
	// 地面描画
	ground_->Draw();

	// モブエネミー描画
	enemyManager->Draw();

	// ボス描画
	if (blaster_) {
		blaster_->Draw();
	}

	// プレイヤー描画
	player_->ObjDraw();

	// スカイボックス描画
	skyBox_->ObjDraw();

	// 弾の描画
	bulletManager_->Draw();

	// パーティクルの描画
	particleManager->Draw();
	objParticleManager->Draw();

	// 建物描画
	// billManager->Draw();
}

void GameScene::SpriteDraw() {
	switch (scene_) {
	case GameScene::Games:
		// ボス登場警告演出
		if (bossWarning_) {
			bossWarning_->Draw();
		}

		if (blaster_) {
			// ボスUI描画
			blaster_->UIDraw();
		}

		// 2D情報描画
		player_->SpriteDraw();

		// UI描画
		player_->UIDraw();

		// スコア描画
		ScoreManager::GetInstance()->Draw();
		break;
	case GameScene::Over:
		// UI描画
		player_->UIDraw();

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

		poseBack_->Draw(
		    TextureManager::GetInstance()->GetTextures("White1x1"), {0, 0}, {width, height}, 0.0f,
		    {0, 0, 0, 0.7f}, false, false, {0, 0});

		poseTexPos_ = {width / 2, height * 1 / 4};
		poseTexS_->Draw(
		    TextureManager::GetInstance()->GetTextures("Pose"), poseTexPos_, {1.5f, 1.5f});

		selectBar_->Draw(
		    TextureManager::GetInstance()->GetTextures("White1x1"), selectBarPos_, {200.0f, 34.0f},
		    0.0f, {0.5f, 0.5f, 0.5f, 0.8f});

		operationS_->Draw(TextureManager::GetInstance()->GetTextures("Operation"), operationPos_);
		backTitleS_->Draw(TextureManager::GetInstance()->GetTextures("Back"), backTitlePos_);
	}
}

void GameScene::Final() { delete collisionManager_; }

void GameScene::CheckAllCollisions() {
	// 登録当たり判定を削除
	collisionManager_->Reset();

#pragma region 準備処理
	// 自機弾の取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets =
	    bulletManager_->GetPlayerBullets();

	// 敵弾の取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bulletManager_->GetEnemyBullets();

	// ボム
	const std::list<std::unique_ptr<Bom>>& boms = bulletManager_->GetBoms();

	// 爆発
	const std::list<std::unique_ptr<Explosion>>& explosions = bulletManager_->GetExplosion();

	// 敵の取得
	const std::list<std::unique_ptr<MobEnemy>>& mobEnemys = enemyManager->GetMobEnemys();

	// 当たり判定を登録
	// 自機
	player_->SetRadius(3.0f);
	collisionManager_->AddCollider(player_.get());

	// 雑魚敵
	for (const std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys) {
		mobEnemy->SetRadius(6.0f);
		collisionManager_->AddCollider(mobEnemy.get());
	}

	// ボス
	if (blaster_) {
		blaster_->SetRadius(12.0f);
		collisionManager_->AddCollider(blaster_.get());
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

		if (blaster_ && !blaster_->GetIsDead() && isBossBattle_ && !player_->GetIsDead()) {
			posA = player_->GetWorldPos();

			for (uint32_t i = 0; i < 8; i++) {
				posB[i] = blaster_->UnitsGetWorldPos(i);

				if (MyCollisions::CheckSphereToSphere(posA, posB[i], 3.0f, 4.0f)) {
					player_->OnCollision(player_.get());
				}
			}
		}
	}

	// レーザーと自機の当たり判定
	{
		// 判定対象AとBの座標
		KMyMath::Vector3 posA;
		std::array<KMyMath::Vector3, 16> posB;

		if (blaster_ && !blaster_->GetIsDead() && isBossBattle_ && !player_->GetIsDead()) {
			posA = player_->GetWorldPos();

			for (size_t i = 0; i < 8; i++) {
				size_t j = i + 8;
				posB[i] = bulletManager_->GetLazersPos(i);
				posB[j] = bulletManager_->GetLazersPos(j);

				if (MyCollisions::CheckBoxToBox(
				        posA, posB[i], {3.0f, 3.0f, 3.0f}, {1.0f, 180.0f, 1.0f})) {
					player_->OnCollision(player_.get());
				}

				if (MyCollisions::CheckBoxToBox(
				        posA, posB[j], {3.0f, 3.0f, 3.0f}, {180.0f, 1.0f, 1.0f})) {
					player_->OnCollision(player_.get());
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
	if (isBossBattle_ || GameManager::GetInstance()->GetMovieFlag("Boss")) {
		return;
	}

	if (!bossWarning_) {
		bool isBossBattleStart = camera_->GetCameraPos().z >= bossBattleStartPos;

		// スタート位置にいなかったらスキップ
		if (!isBossBattleStart) {
			return;
		}

		// ステージBGM停止
		audioManager_->SoundStopWave("BattleBGM.wav");

		// カメラ前進止める
		camera_->SetIsAdvance(false);

		// ビルを全部動かす状態へ
		billManager->SetIsAdvance(true);

		// 地面を動かす状態へ
		ground_->SetIsAdvance(true);

		// ボス登場警告作成
		bossWarning_ = std::make_unique<Warning>();
		bossWarning_->Init();

		// 敵を全削除
		enemyManager->AllEnemyDelete();
	} else {
		// 演出が終わってないときは抜ける
		if (!bossWarning_->GetIsDelete()) {
			return;
		}

		//
		sceneChange->SceneChangeStart();

		// ボス登場警告解放
		bossWarning_.reset();

		// ボス配置
		const float bossDistance = 150;
		const KMyMath::Vector3 bossBasePos = {0.0f, 120.0f, bossBattleStartPos + bossDistance};

		// 生成
		blaster_.reset(Blaster::Create(
		    PipelineManager::GetInstance()->GetPipeline("Obj"), bossBasePos,
		    PipelineManager::GetInstance()->GetPipeline("Sprite")));

		Blaster::nowBlaster = blaster_.get();

		// ボス出現ムービーへ
		GameManager::GetInstance()->SetMovieFlag(true, "Boss");
		movie_ = std::make_unique<BossStart>();
		scene_ = Movies;

		isWarnning = true;
	}
}

void GameScene::PlayerDead() {
	if (player_->GetIsDead()) {
		// 全ての敵を消去
		enemyManager->AllEnemyDelete();
		scene_ = Over;
	}
}

void GameScene::GameOverMovie()
{
	if (!isCallDeadCamera) {
		// 撃墜カメラ呼び出し
		camera_->CallCrash();
		isCallDeadCamera = true;
		// プレイヤーとカメラの接続解除
		player_->SetParent(nullptr);
		player_->SetPos(player_->GetWorldPos());
		// 撃墜ムービーへ
		GameManager::GetInstance()->SetMovieFlag(true, "Over");
	}

	if (player_->GetIsFallEffectEnd()) {
		GoGameOverScene();
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
		bulletManager_->AllBulletDelete();
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

		if (!blaster_) {
			// エネミーマネージャーの更新
			enemyManager->Update(camera_->GetViewPro(), camera_->GetWorldPos());
		}

		// 天箱を自機に追従
		skyBox_->SetPosZ(player_->GetWorldPos().z);
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
				isBossBattle_ = true;
			}

			scene_ = Games;
			movie_->SetIsFinish(false);
		}

		// ボスの更新
		if (blaster_) {
			if (blaster_->GetIsFallEffectEnd()) {
				goClearMovieTimer++;
				if (goClearMovieTimer == goClearMovieTime) {
					player_->SetParent(nullptr);
					player_->SetPos(player_->GetWorldPos());
					gameManager_->SetMovieFlag(true, "Clear");
					movie_ = std::make_unique<StageClear>();
					scene_ = Movies;
					goClearMovieTimer = goClearMovieTime + 1.0f;
				}
			}

			blaster_->Update(
			    camera_->GetViewPro(), camera_->GetWorldPos(), gameManager_->GetMovieFlag("Boss"));
			Blaster::nowBlaster = blaster_.get();
		}

		// プレイヤーの更新
		player_->Update(camera_->GetViewPro(), camera_->GetWorldPos());
		Player::nowPlayer = player_.get();

		// 弾の更新
		bulletManager_->Update(camera_->GetViewPro(), camera_->GetWorldPos());

		// 地面の更新
		ground_->Update(camera_->GetViewPro(), camera_->GetWorldPos());

		// スカイボックスの更新
		skyBox_->Update(camera_->GetViewPro(), camera_->GetWorldPos());

		// パーティクルマネージャーの更新
		particleManager->Update(camera_->GetViewPro());
		objParticleManager->Update(camera_->GetViewPro(), camera_->GetWorldPos());

		// ビルマネージャー
		billManager->Update(
		    camera_->GetViewPro(), camera_->GetWorldPos(), camera_->GetCameraPos().z - 20.0f);

		// カメラの更新
		camera_->Update(
		    gameManager_->GetMovieFlag("Start"), gameManager_->GetMovieFlag("Boss"),
		    gameManager_->GetMovieFlag("Clear"));
		RailCamera::nowRailCamera = camera_.get();

		ScoreManager::GetInstance()->Update();

		light_->Update();

		// ボス登場警告
		if (bossWarning_) {
			bossWarning_->Update();
		}
	} else {
		PoseAction();
	}

	gameManager_->Update();
}

void GameScene::PoseAction() {

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	operationPos_ = {width / 2, (height / 2) - 60.0f};
	backTitlePos_ = {width / 2, height / 2};

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
		selectBarPos_ = operationPos_;

		if (input->GetPadButton(XINPUT_GAMEPAD_A)) {
		}
	}

	if (isBackTitle) {
		selectBarPos_ = backTitlePos_;

		if (input->GetPadButton(XINPUT_GAMEPAD_A)) {
			audioManager_->SoundStopWave("BattleBGM.wav");
			audioManager_->SoundStopWave("bossBGM.wav");
			sceneChange->SceneChangeStart();
		}
	}

	if (sceneChange->GetIsChange()) {
		gameManager_->SetIsStartMovie(false);
		sceneManager->ChangeScene("TITLE");
		bulletManager_->AllBulletDelete();
	}
}
