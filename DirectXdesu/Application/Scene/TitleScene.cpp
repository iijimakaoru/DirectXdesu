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
	BaseScene::Init();

	// インスタンス
	input = KInput::GetInstance();

	light_.reset(Light::Create());
	light_->SetLightRGB({1.0f, 1.0f, 1.0f});
	KObject3d::SetLight(light_.get());

	// カメラ読み込み
	camera = std::make_unique<GameCamera>();
	camera->Init();

	sceneManager = SceneManager::GetInstance();

	skyDome.reset(
	    KObject3d::Create(skyDomeModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	skyDome->GetTransform().SetScale({400.0f, 400.0f, 400.0f});

	audioManager = AudioManager::GetInstance();
}

void TitleScene::Update() {
	light_->Update();

	skyDome->Update(camera->GetViewPro(), camera->GetWorldPos());

	camera->Update();
	
	GoNextScene();
}

void TitleScene::ObjDraw() {
	skyDome->Draw();
}

void TitleScene::SpriteDraw()
{
}

void TitleScene::Final() {

}

void TitleScene::GoNextScene() {
	if (input->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
		sceneManager->ChangeScene("GAME");
	}
	else if (input->IsTrigger(DIK_SPACE)) {
		sceneManager->ChangeScene("GAME");
	}
}
