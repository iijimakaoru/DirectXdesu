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
#include "Ease.h"
#include "PipelineManager.h"
#include "ModelManager.h"
#include "TextureManager.h"

GameScene::~GameScene() { Final(); };

void GameScene::LoadResources() {}

void GameScene::Init() {
	BaseScene::Init();

	// インスタンス
	input = KInput::GetInstance();

	light_.reset(Light::Create());
	light_->SetLightRGB({1, 1, 1});
	KObject3d::SetLight(light_.get());

	// カメラ生成
	camera_ = std::make_unique<GameCamera>();

	hogeModel_ = std::make_unique<Cube>();
	hogeModel_->Init();
	
	hogeObject_.reset(KObject3d::Create(hogeModel_.get(),
		PipelineManager::GetInstance()->GetPipeline("Obj")));
	

	// シーンマネージャーインスタンス
	sceneManager = SceneManager::GetInstance();

	// カメラ初期化
	camera_->Init();

	audioManager_ = AudioManager::GetInstance();
}

void GameScene::Update() {
	light_->SetLightRGB({lightRGB_.x, lightRGB_.y, lightRGB_.z});
	light_->SetLightDir({lightDir_.x, lightDir_.y, lightDir_.z, 0.0f});

	hogeObject_->GetTransform().AddSetRot({ 0,0,1 });
	hogeObject_->Update(camera_->GetViewPro(),camera_->GetWorldPos());

	camera_->Update();
}

void GameScene::ObjDraw() {
	hogeObject_->Draw(TextureManager::GetInstance()->GetTextures("MESI"));
}

void GameScene::SpriteDraw() {
	
}

void GameScene::Final() {  }