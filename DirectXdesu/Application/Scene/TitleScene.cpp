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
	// 天球モデル
	skyDomeModel = ModelManager::GetInstance()->GetModels("S_SkyDorm");
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

	skyDome.reset(
	    KObject3d::Create(skyDomeModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	skyDome->GetTransform().SetScale({400.0f, 400.0f, 400.0f});
	skyDome->SetColor({ 0,0,0,0 });

	audioManager = AudioManager::GetInstance();

	emitter_ = new Emitter(
		100,
		1,
		100.0f,
		300.0f,
		DirectX::XMFLOAT3(0.0f,0.0f,0.0f),
		DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f),
		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f)
	);

	meshEmitter_ = new MeshEmitter(
		50.0f,
		300.0f,
		DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f),
		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f)
	);

	meshGpuParticle_ = new MeshGPUParticle(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro(),
		emitter_,"suzanne1");

	particleEditor_ = new ParticleEditor();
}

void TitleScene::Update() {
	ImGui::Begin("MeshParticle");
	ImGui::SliderFloat3("Position", &position.x, -6, 6, "%.1f");
	ImGui::SliderFloat3("Rotation", &rotation.x, -5, 5, "%.1f");
	ImGui::SliderFloat3("Scaling", &scaling.x, -5, 5, "%.1f");
	ImGui::End();

	emitter_->SetPosition(position);
	emitter_->SetRotation(rotation);
	emitter_->SetScaling(scaling);

	timer_.UpdateTimer();
	timer_.UpdateTitleBarStats();

	light_->SetLightRGB({lightRGB.x, lightRGB.y, lightRGB.z});
	light_->SetLightDir({lightDir.x, lightDir.y, lightDir.z, 0.0f});

	light_->Update();

	skyDome->Update(camera->GetViewPro(), camera->GetWorldPos());

	//particleEditor_->Update();

	if (input->IsTrigger(DIK_1)) {
		
	}

	meshGpuParticle_->Update(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro(),
		emitter_);

	camera->Update();
}

void TitleScene::ObjDraw() {
	skyDome->Draw();

	meshGpuParticle_->Draw(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro(),
		emitter_);
}

void TitleScene::SpriteDraw() {
	
}

void TitleScene::Final() {
	delete emitter_;
	delete meshEmitter_;
	delete gpuParticle_;
	delete meshGpuParticle_;
	delete particleEditor_;
}

void TitleScene::StartScene() {
	
}

void TitleScene::GoNextScene() {
	
}

void TitleScene::TitleCall() {
	
}
