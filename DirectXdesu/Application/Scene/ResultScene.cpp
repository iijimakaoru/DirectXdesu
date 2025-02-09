#include "ResultScene.h"
#include "SceneManager.h"

#include "ModelManager.h"
#include "PipelineManager.h"

void ResultScene::LoadResources()
{
	skydomeModel = ModelManager::GetInstance()->GetModels("S_SkyDorm");
	stageModel = ModelManager::GetInstance()->GetModels("clearStage");
	foxModel = ModelManager::GetInstance()->GetModels("clearFox");

	texThanks = TextureManager::Load("Resources/texture/thanks.png");
	texPress = TextureManager::Load("Resources/texture/pressA.png");
}

void ResultScene::Init()
{
	BaseScene::Init();

	// インスタンス
	input = KInput::GetInstance();

	light_.reset(Light::Create());
	light_->SetLightRGB({ 1.0f, 1.0f, 1.0f });
	KObject3d::SetLight(light_.get());

	// カメラ読み込み
	camera = std::make_unique<GameCamera>();
	camera->Init();
	camera->SetCameraPos({-3.8f,2.5f,-1.0f });
	camera->SetCameraRot({0.0f,45.0f,0.0f});

	sceneManager = SceneManager::GetInstance();

	skydome.reset(KObject3d::Create(skydomeModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	skydome->GetTransform().SetScale({ skydomeSize, skydomeSize, skydomeSize });
	skydome->GetTransform().SetPos({ 0.0f, 100.0f, 500.0f });

	stage.reset(KObject3d::Create(stageModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	fox.reset(KObject3d::Create(foxModel, PipelineManager::GetInstance()->GetPipeline("Obj")));

	thanks.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	press.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
}

void ResultScene::Update()
{
	skydome->Update(camera->GetViewPro(), camera->GetWorldPos());
	stage->Update(camera->GetViewPro(), camera->GetWorldPos());
	fox->Update(camera->GetViewPro(), camera->GetWorldPos());

	camera->Update();

	if (input->GetPadButtonDown(A)) {
		sceneManager->ChangeScene("TITLE");
	}
	else if (input->IsTrigger(DIK_SPACE)) {
		sceneManager->ChangeScene("TITLE");
	}
}

void ResultScene::ObjDraw()
{
	skydome->Draw();
	stage->Draw();
	fox->Draw();
}

void ResultScene::SpriteDraw()
{
	thanks->Draw(texThanks, { 880.0f, 300.0f});
	press->Draw(texPress, { 880.0f ,480.0f }, { 0.5f,0.5f });
}

void ResultScene::Final()
{
}
