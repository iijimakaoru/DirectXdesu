#include "ResultScene.h"
#include "SceneManager.h"

#include "ModelManager.h"
#include "PipelineManager.h"

void ResultScene::LoadResources()
{
	skydomeModel = ModelManager::GetInstance()->GetModels("S_SkyDorm");
	stageModel = ModelManager::GetInstance()->GetModels("clearStage");
	foxModel = ModelManager::GetInstance()->GetModels("clearFox");
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
	camera->SetCameraPos({1,2,-5 });

	sceneManager = SceneManager::GetInstance();

	skydome.reset(KObject3d::Create(skydomeModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	skydome->GetTransform().SetScale({ skydomeSize, skydomeSize, skydomeSize });
	skydome->GetTransform().SetPos({ 0.0f, 100.0f, 500.0f });

	stage.reset(KObject3d::Create(stageModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	fox.reset(KObject3d::Create(foxModel, PipelineManager::GetInstance()->GetPipeline("Obj")));

}

void ResultScene::Update()
{
	skydome->Update(camera->GetViewPro(), camera->GetWorldPos());
	stage->Update(camera->GetViewPro(), camera->GetWorldPos());
	fox->Update(camera->GetViewPro(), camera->GetWorldPos());

	camera->Update();
}

void ResultScene::ObjDraw()
{
	skydome->Draw();
	stage->Draw();
	fox->Draw();
}

void ResultScene::SpriteDraw()
{
}

void ResultScene::Final()
{
}
