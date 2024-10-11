#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

#include "Ease.h"

#include "ModelManager.h"
#include "PipelineManager.h"

#include "PostEffectManager.h"

TitleScene::~TitleScene() { Final(); }

void TitleScene::LoadResources() {
	// 機体モデル
	model = ModelManager::GetInstance()->GetModels("Player");

	// 天球モデル
	skyDomeModel = ModelManager::GetInstance()->GetModels("T_SkyDorm");
}

void TitleScene::Init() {
	BaseScene::Init();

	// インスタンス
	input = KInput::GetInstance();

	light_.reset(Light::Create());
	light_->SetLightRGB({1.0f, 1.0f, 1.0f});
	light_->SetLightDir({0, -1, 0, 0.0f});
	KObject3d::SetLight(light_.get());

	// カメラ読み込み
	camera = std::make_unique<TitleCamera>();
	camera->Init();

	sceneManager = SceneManager::GetInstance();

	// タイトル名
	titlePos = {width / 2, height * 1 / 3};
	mesi.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	shooter.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	shooterScale = {1.0f, 1.0f};

	mold.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	moldScale = {1.6f, 1.6f};

	// プッシュA
	pushA.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	pushAPos = {width / 2, height * 4 / 5};

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, PipelineManager::GetInstance()->GetPipeline("Obj")));
	object3d->GetTransform().SetScale({0.0f, 0.0f, 0.0f});

	skyDome.reset(
	    KObject3d::Create(skyDomeModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	skyDome->GetTransform().SetScale({200.0f, 200.0f, 200.0f});

	// フラッシュ
	flash.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	flashAlpha = 0;

	startScene = true;

	startScenePhase = 0;

	phaseTimer = 0;

	phaseTime = 0;

	goGame = false;

	goGamePhase = 0;

	texEaseTimer = 0;

	objEaseTimer = 0;

	audioManager = AudioManager::GetInstance();
}

void TitleScene::Update() {
	light_->SetLightRGB({lightRGB.x, lightRGB.y, lightRGB.z});
	light_->SetLightDir({lightDir.x, lightDir.y, lightDir.z, 0.0f});

	if (startScene) {
		StartScene();
	} else {
		if (flashAlpha > 0) {
			flashAlpha -= 0.1f;
		}

		if (sceneChange->GetIsChange()) {
			// シーン切り替え依頼
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}

	light_->Update();

	object3d->Update(camera->GetViewPro(), camera->GetWorldPos());

	skyDome->Update(camera->GetViewPro(), camera->GetWorldPos());

	camera->Update();
}

void TitleScene::ObjDraw() {
	object3d->Draw();

	skyDome->Draw();
}

void TitleScene::SpriteDraw() {
	if (!startScene) {
		mold->Draw(
		    TextureManager::GetInstance()->GetTextures("Mold"), titlePos + moldPos, moldScale);

		mesi->Draw(
		    TextureManager::GetInstance()->GetTextures("MESI"), titlePos + mesiPos, mesiScale);

		shooter->Draw(
		    TextureManager::GetInstance()->GetTextures("SHOOTER"), titlePos + shooterPos,
		    shooterScale);

		pushA->Draw(TextureManager::GetInstance()->GetTextures("PushA"), pushAPos, {0.75f, 0.75f});
	} else {
		mold->Draw(
		    TextureManager::GetInstance()->GetTextures("Mold"), titlePos + moldPos, moldScale);

		shooter->Draw(
		    TextureManager::GetInstance()->GetTextures("SHOOTER"), titlePos + shooterPos,
		    shooterScale);

		mesi->Draw(
		    TextureManager::GetInstance()->GetTextures("MESI"), titlePos + mesiPos, mesiScale);
	}

	flash->Draw(
	    TextureManager::GetInstance()->GetTextures("White1x1"), {width / 2, height / 2},
	    {width, height}, 0, {1.0f, 1.0f, 1.0f, flashAlpha});
}

void TitleScene::Final() {}

void TitleScene::StartScene() {
	// シーン遷移待ち
	if (startScenePhase == 0) {
		phaseTime = 30;

		phaseTimer++;

		if (phaseTimer > phaseTime) {
			startScenePhase++;
			phaseTimer = 0;
		}
	}
	// こっちに飛んできて上に行く
	else if (startScenePhase == 1) {
		start = {0, -20, 190};
		p1 = {0, -20, 20};
		p2 = {0, -30, -30};
		end = {0, 50, -30};

		phaseTime = 90;

		phaseTimer++;

		if (phaseTimer == 30) {
			audioManager->SEPlay_wav("flySE.wav");
		}

		// ポイント１の制御点
		KMyMath::Vector3 point1_1 = MyEase::Lerp3D(start, p1, phaseTimer / phaseTime);
		KMyMath::Vector3 point1_2 = MyEase::Lerp3D(p1, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point1 = MyEase::Lerp3D(point1_1, point1_2, phaseTimer / phaseTime);

		// ポイント２の制御点
		KMyMath::Vector3 point2_1 = MyEase::Lerp3D(start, p2, phaseTimer / phaseTime);
		KMyMath::Vector3 point2_2 = MyEase::Lerp3D(p2, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point2 = MyEase::Lerp3D(point2_1, point2_2, phaseTimer / phaseTime);

		object3d->GetTransform().SetPos(MyEase::Lerp3D(point1, point2, phaseTimer / phaseTime));

		object3d->GetTransform().SetRot(
		    {MyEase::Lerp(0.0f, -45.0f, phaseTimer / phaseTime), 180.0f,
		     object3d->GetTransform().GetRot().z});

		if (objEaseTimer < objEaseTime) {
			objEaseTimer++;

			object3d->GetTransform().SetScale(
			    {MyEase::OutCubicFloat(0, 1, objEaseTimer / objEaseTime),
			     MyEase::OutCubicFloat(0, 1, objEaseTimer / objEaseTime),
			     MyEase::OutCubicFloat(0, 1, objEaseTimer / objEaseTime)});
		}

		if (phaseTimer > phaseTime) {
			startScenePhase++;
			phaseTimer = 0;
		}
	}
	// 上から帰ってくる
	else if (startScenePhase == 2) {
		start = {0, 80, 80};
		p1 = {0, 50, 80};
		p2 = {0, 0, 60};
		end = {0, -3, 30};

		phaseTime = 45;

		phaseTimer++;

		// ベジエ曲線
		// ポイント１の制御点
		KMyMath::Vector3 point1_1 = MyEase::Lerp3D(start, p1, phaseTimer / phaseTime);
		KMyMath::Vector3 point1_2 = MyEase::Lerp3D(p1, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point1 = MyEase::Lerp3D(point1_1, point1_2, phaseTimer / phaseTime);

		// ポイント２の制御点
		KMyMath::Vector3 point2_1 = MyEase::Lerp3D(start, p2, phaseTimer / phaseTime);
		KMyMath::Vector3 point2_2 = MyEase::Lerp3D(p2, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point2 = MyEase::Lerp3D(point2_1, point2_2, phaseTimer / phaseTime);

		object3d->GetTransform().SetPos(MyEase::Lerp3D(point1, point2, phaseTimer / phaseTime));

		object3d->GetTransform().SetRot(
		    {MyEase::Lerp(45.0f, 0.0f, phaseTimer / phaseTime), 180.0f,
		     object3d->GetTransform().GetRot().z});

		isTitle = true;

		if (phaseTimer > phaseTime) {
			startScenePhase++;
			phaseTimer = 0;
		}
	}
	// 余韻
	else if (startScenePhase == 3) {
		phaseTime = 30;

		phaseTimer++;

		object3d->GetTransform().SetPos(
		    {object3d->GetTransform().GetPos().x, object3d->GetTransform().GetPos().y,
		     MyEase::OutCubicFloat(30, 0, phaseTimer / phaseTime)});

		object3d->GetTransform().SetRot(
		    {object3d->GetTransform().GetRot().x, object3d->GetTransform().GetRot().y,
		     MyEase::OutCubicFloat(0, 360, phaseTimer / phaseTime)});

		if (phaseTimer > phaseTime) {
			startScenePhase++;
			phaseTimer = 0;
		}
	} else if (startScenePhase == 4) {
		// フラッシュSE
		audioManager->SEPlay_wav("flashSE.wav");

		// フラッシュ！
		flashAlpha = 1.0f;

		// オブジェクト情報タイトルどうりに
		object3d->GetTransform().SetPos({0, 0, 0});
		object3d->GetTransform().SetRot({0, 0, 0});
		object3d->GetTransform().SetScale({1, 1, 1});

		// タイトル位置
		shooterPos.x = 200;
		moldPos.x = 0;
		mesiPos = {-200, -30};
		mesiScale = {1.0f, 1.0f};

		// タイマー初期化
		phaseTimer = 0;

		startScenePhase++;
	}
	// スタート画面
	else {
		audioManager->BGMPlay_wav("titleBGM.wav");
		camera->StartRound();
		startScene = false;
	}

	if (isTitle) {
		TitleCall();
	}

	// タイトル導入スキップ
	if (startScenePhase < 4) {
		if (input->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
			startScenePhase = 4;
			isTitle = false;
		}
	}
}

void TitleScene::GoNextScene() {
	// カメラ遷移待ち
	if (goGamePhase == 0) {
		audioManager->SoundStopWave("titleBGM.wav");

		phaseTime = 45;

		if (texEaseTimer < texEaseTime) {
			texEaseTimer++;

			titlePos.y = MyEase::InQuadFloat(
			    height * 1 / 3, (height * 1 / 3) - 500, texEaseTimer / texEaseTime);

			pushAPos.y = MyEase::InQuadFloat(
			    height * 2 / 3, (height * 2 / 3) + 300, texEaseTimer / texEaseTime);
		}

		if (phaseTimer < phaseTime) {
			phaseTimer++;
		} else {
			goGamePhase++;
			phaseTimer = 0;
		}
	} else if (goGamePhase == 1) {
		phaseTime = 30;

		if (phaseTimer < phaseTime) {
			phaseTimer++;
		} else {
			goGamePhase++;
			phaseTimer = 0;
		}
	} else if (goGamePhase == 2) {
		phaseTime = 15;

		start = {0, 0, 0};
		p1 = {0, 0, 100};
		p2 = {0, 0, 150};
		end = {0, 60, 180};

		if (phaseTimer < phaseTime) {
			if (phaseTimer == 0) {
				audioManager->SEPlay_wav("flySE.wav");
			}

			phaseTimer++;

			object3d->GetTransform().SetScale(
			    {MyEase::OutCubicFloat(1, 0, phaseTimer / phaseTime),
			     MyEase::OutCubicFloat(1, 0, phaseTimer / phaseTime),
			     MyEase::OutCubicFloat(1, 0, phaseTimer / phaseTime)});

			// ポイント１の制御点
			KMyMath::Vector3 point1_1 = MyEase::Lerp3D(start, p1, phaseTimer / phaseTime);
			KMyMath::Vector3 point1_2 = MyEase::Lerp3D(p1, end, phaseTimer / phaseTime);
			KMyMath::Vector3 point1 = MyEase::Lerp3D(point1_1, point1_2, phaseTimer / phaseTime);

			// ポイント２の制御点
			KMyMath::Vector3 point2_1 = MyEase::Lerp3D(start, p2, phaseTimer / phaseTime);
			KMyMath::Vector3 point2_2 = MyEase::Lerp3D(p2, end, phaseTimer / phaseTime);
			KMyMath::Vector3 point2 = MyEase::Lerp3D(point2_1, point2_2, phaseTimer / phaseTime);

			object3d->GetTransform().SetPos(MyEase::Lerp3D(point1, point2, phaseTimer / phaseTime));
		} else {
			goGamePhase++;
			phaseTimer = 0;
		}
	} else if (goGamePhase == 3) {
		sceneChange->SceneChangeStart();
		goGamePhase++;
	}
}

void TitleScene::TitleCall() {
	if (titlePhase == 0) {
		titlePhaseTime = 30;

		if (titlePhaseTimer < titlePhaseTime) {
			titlePhaseTimer++;

			shooterPos.x = MyEase::OutCubicFloat(1200, 200, titlePhaseTimer / titlePhaseTime);
			moldPos.x = MyEase::OutCubicFloat(-1200, 0, titlePhaseTimer / titlePhaseTime);
		} else {
			titlePhase++;
			titlePhaseTimer = 0;
		}
	} else if (titlePhase == 1) {
		titlePhaseTime = 45;

		if (titlePhaseTimer < titlePhaseTime) {
			titlePhaseTimer++;

			mesiPos = MyEase::InCubicVec2({0, 0}, {-200, -30}, titlePhaseTimer / titlePhaseTime);
			mesiScale =
			    MyEase::InCubicVec2({2.5f, 2.5f}, {1.0f, 1.0f}, titlePhaseTimer / titlePhaseTime);
		} else {
			titlePhase++;
			titlePhaseTimer = 0;
		}
	} else {
		isTitle = false;
	}
}
