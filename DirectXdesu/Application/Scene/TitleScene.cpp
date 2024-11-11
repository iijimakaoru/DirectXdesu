#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

#include "Ease.h"

#include "ModelManager.h"
#include "PipelineManager.h"

#include "PostEffectManager.h"

const int gNumberFrameResources = 3;

TitleScene::~TitleScene() { Final(); }

void TitleScene::LoadResources() {
	// 機体モデル
	model = ModelManager::GetInstance()->GetModels("Player");

	// 天球モデル
	skyDomeModel = ModelManager::GetInstance()->GetModels("T_SkyDorm");
}

void TitleScene::Init() {
	timer_ = Timer(KWinApp::GetHWND(), KWinApp::GetWindow().lpszMenuName);

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

	audioManager = AudioManager::GetInstance();

	gpuParticle_ = new GPUParticle(timer_, camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro(),
		30000,
		100,
		10000.0f,
		10.0f,
		DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f),
		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));

	gpuParticle2_ = new GPUParticle(timer_, camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro(),
		10000,
		100,
		10000.0f,
		40.0f,
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

	camera->StartRound();
}

void TitleScene::Update() {
	timer_.UpdateTimer();
	timer_.UpdateTitleBarStats();

	light_->SetLightRGB({lightRGB.x, lightRGB.y, lightRGB.z});
	light_->SetLightDir({lightDir.x, lightDir.y, lightDir.z, 0.0f});

	light_->Update();

	object3d->Update(camera->GetViewPro(), camera->GetWorldPos());

	skyDome->Update(camera->GetViewPro(), camera->GetWorldPos());

	if (input->IsTrigger(DIK_1)) {
		gpuParticle2_ = new GPUParticle(
			timer_, 
			camera->GetViewPro()->GetMatView(), 
			camera->GetViewPro()->GetMatPro(),
			10000,
			100,
			10000.0f,
			40.0f,
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),
			DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	}

	gpuParticle_->Update(timer_, 
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro());

	gpuParticle2_->Update(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro());

	camera->Update();
}

void TitleScene::ObjDraw() {
	object3d->Draw();

	skyDome->Draw();

	gpuParticle_->Draw(timer_, 
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro());

	gpuParticle2_->Draw(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro());
}

void TitleScene::SpriteDraw() {
	
}

void TitleScene::Final() {
	delete gpuParticle_;
	delete gpuParticle2_;
}

void TitleScene::StartScene() {
	
}

void TitleScene::GoNextScene() {
	
}

void TitleScene::TitleCall() {
	
}
