#include "ClearScene.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include "Ease.h"

#include <imgui.h>

#include "PipelineManager.h"

ClearScene::~ClearScene() { Final(); }

void ClearScene::LoadResources() {}

void ClearScene::Init() {
	BaseScene::Init();

	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();
}

void ClearScene::Update() {
	
}

void ClearScene::ObjDraw() {}

void ClearScene::SpriteDraw() {
	
}

void ClearScene::Final() {}