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

	emitter_ = new Emitter(100, 1, 1.0f, 2.5f, 0.025f,
		DirectX::XMFLOAT3(5.0f,0.0f,0.0f),
		DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f,	0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f)
	);

	meshModel_ = std::make_unique<MeshModel>("suzanne1");
	meshGpuParticle_ = new MeshGPUParticle(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro(),
		emitter_,
		meshModel_.get());

	arrowEmitter_ = std::make_unique<Emitter>(100, 1, 1.0f, 2.5f, 0.025f,
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	arrowModel_ = std::make_unique<MeshModel>("arrowEffect");
	arrowEffect_ = std::make_unique<MeshGPUParticle>(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro(),
		emitter_,
		arrowModel_.get());

	KMyMath::Vector3 testPos = { 0,0,0 };
	KMyMath::Vector3 testRot = { 0,0,0 };
	KMyMath::Vector3 testScale = { 10,10,10 };
	KMyMath::Vector4 testColor = { 1,1,1,1 };
	float testLimit = 60.0f;
	arrow_.reset(ArrowEffect::Create(testPos, testRot, testScale, testColor, arrowModel_.get(), testLimit,
		timer_, camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro()));

	sprite.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	texData = TextureManager::GetInstance()->GetTextures("Texture");
}

void TitleScene::Update() {
	ImGui::Begin("MeshParticle");
	ImGui::SliderFloat3("Position", &position.x, -6, 6, "%.1f");
	ImGui::SliderFloat3("Rotation", &rotation.x, -180, 180, "%.1f");
	ImGui::SliderFloat3("Scaling", &scaling.x, -2, 2, "%.1f");
	ImGui::End();

	arrowEmitter_->SetPosition(position);
	arrowEmitter_->SetRotation(rotation);
	arrowEmitter_->SetScaling(scaling);

	timer_.UpdateTimer();
	timer_.UpdateTitleBarStats();

	light_->SetLightRGB({lightRGB.x, lightRGB.y, lightRGB.z});
	light_->SetLightDir({lightDir.x, lightDir.y, lightDir.z, 0.0f});

	light_->Update();

	skyDome->Update(camera->GetViewPro(), camera->GetWorldPos());

	//particleEditor_->Update();

	if (input->IsTrigger(DIK_1)) {
		std::unique_ptr<ArrowEffect> newArrowEffect;
		KMyMath::Vector3 testPos = MyMathConvert::ChangeXMFloat3toVector3(position);
		KMyMath::Vector3 testRot = MyMathConvert::ChangeXMFloat3toVector3(rotation);
		KMyMath::Vector3 testScale = MyMathConvert::ChangeXMFloat3toVector3(scaling);
		KMyMath::Vector4 testColor = { 1,1,1,1 };
		float testLimit = 60.0f;
		newArrowEffect.reset(ArrowEffect::Create(testPos, testRot, testScale, testColor, arrowModel_.get(), testLimit,
			timer_, camera->GetViewPro()->GetMatView(), camera->GetViewPro()->GetMatPro()));

		arrowEffects.push_back(std::move(newArrowEffect));
	}

	meshGpuParticle_->Update(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro(),
		emitter_);

	arrowEffect_->Update(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro(),
		arrowEmitter_.get());

	arrow_->Update(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro());

	for (std::unique_ptr<ArrowEffect>& arrowEffect : arrowEffects) 
	{
		arrowEffect->Update(timer_,
			camera->GetViewPro()->GetMatView(),
			camera->GetViewPro()->GetMatPro());
	}

	camera->Update();
}

void TitleScene::ObjDraw() {
	skyDome->Draw();

	meshGpuParticle_->Draw(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro(),
		emitter_);

	/*arrowEffect_->Draw(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro(),
		arrowEmitter_.get());*/

	/*arrow_->Draw(timer_,
		camera->GetViewPro()->GetMatView(),
		camera->GetViewPro()->GetMatPro());*/

	for (std::unique_ptr<ArrowEffect>& arrowEffect : arrowEffects) 
	{
		arrowEffect->Draw(timer_,
			camera->GetViewPro()->GetMatView(),
			camera->GetViewPro()->GetMatPro());
	}
}

void TitleScene::SpriteDraw() {
	sprite->Draw(
		texData, {0,0}, {32,32}, 0,
		{1,0,0,1}, false, false, {0,0});
}

void TitleScene::Final() {
	delete emitter_;
	delete gpuParticle_;
	delete meshGpuParticle_;
}

void TitleScene::StartScene() {
	
}

void TitleScene::GoNextScene() {
	
}

void TitleScene::TitleCall() {
	
}
