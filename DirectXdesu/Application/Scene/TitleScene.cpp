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

	camera->StartRound();
}

void TitleScene::Update() {
	light_->SetLightRGB({lightRGB.x, lightRGB.y, lightRGB.z});
	light_->SetLightDir({lightDir.x, lightDir.y, lightDir.z, 0.0f});

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
	
}

void TitleScene::Final() {}

void TitleScene::StartScene() {
	
}

void TitleScene::GoNextScene() {
	
}

void TitleScene::TitleCall() {
	
}
