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
	// 天球モデル
	skyDomeModel = ModelManager::GetInstance()->GetModels("S_SkyDorm");
}

void TitleScene::Init() {
	timer_ = std::make_unique<Timer>();

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
		10000, 1, 1000.0f, 10.0f, 0.025f,
		DirectX::XMFLOAT3(0.0f,0.0f,0.0f),
		DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f,	0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f)
	);

	testM = std::make_unique<MeshModel>("GroundEffect");
	testP = std::make_unique<MeshGPUParticle>(timer_.get(), camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro(), emitter_, testM.get());
	

	sprite.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	texData = TextureManager::GetInstance()->GetTextures("Texture");
}

void TitleScene::Update() {
	ImGui::Begin("MeshParticle");
	ImGui::SliderFloat3("Position", &position.x, -6, 6, "%.1f");
	ImGui::SliderFloat3("Rotation", &rotation.x, -180, 180, "%.1f");
	ImGui::SliderFloat3("Scaling", &scaling.x, -2, 2, "%.1f");
	ImGui::End();

	timer_->UpdateTimer();

	light_->SetLightRGB({lightRGB.x, lightRGB.y, lightRGB.z});
	light_->SetLightDir({lightDir.x, lightDir.y, lightDir.z, 0.0f});

	light_->Update();

	skyDome->Update(camera->GetViewPro(), camera->GetWorldPos());

	//particleEditor_->Update();

	if (input->IsTrigger(DIK_1)) 
	{
		
	}

	testP->Update(timer_.get(), camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro(), emitter_);

	camera->Update();
}

void TitleScene::ObjDraw() {
	skyDome->Draw();

	testP->Draw(timer_.get(), camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro(), emitter_);
}

void TitleScene::SpriteDraw() {
	sprite->Draw(
		texData, {0,0}, {32,32}, 0,
		{1,0,0,1}, false, false, {0,0});
}

void TitleScene::Final() {
	delete emitter_;
}

void TitleScene::StartScene() {
	
}

void TitleScene::GoNextScene() {
	
}

void TitleScene::TitleCall() {
	
}
