#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

#include "Ease.h"

#include "PipelineManager.h"

#include "ResourceManager.h"

TitleScene::~TitleScene() { Final(); }

void TitleScene::LoadResources() { model_ = std::make_unique<MtlObj>("BattleShip"); }

void TitleScene::Init() {
	object_.reset(
	    KObject3d::Create(model_.get(), PipelineManager::GetInstance()->GetObjPipeline()));
}

void TitleScene::Update() { object_->Update(); }

void TitleScene::ObjDraw() { object_->Draw(); }

void TitleScene::SpriteDraw() {}

void TitleScene::Final() {}

void TitleScene::StartScene() {}

void TitleScene::GoNextScene() {}

void TitleScene::TitleCall() {}
