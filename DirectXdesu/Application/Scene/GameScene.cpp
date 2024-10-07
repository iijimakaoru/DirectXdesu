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

	// シーンマネージャーインスタンス
	sceneManager = SceneManager::GetInstance();

	// カメラ初期化
	camera_->Init();

	audioManager_ = AudioManager::GetInstance();
}

void GameScene::Update() {
	light_->SetLightRGB({lightRGB_.x, lightRGB_.y, lightRGB_.z});
	light_->SetLightDir({lightDir_.x, lightDir_.y, lightDir_.z, 0.0f});
}

void GameScene::ObjDraw() {
	
}

void GameScene::SpriteDraw() {
	
}

void GameScene::Final() {  }