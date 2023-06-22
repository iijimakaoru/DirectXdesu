#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

TitleScene::~TitleScene()
{
	Final();
}

void TitleScene::LoadResources()
{
	
}

void TitleScene::Init()
{
	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();
}

void TitleScene::Update()
{
	camera->Update();

	// ���̃V�[����
	if (input->IsTrigger(DIK_SPACE))
	{
		// �V�[���؂�ւ��˗�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	
}

void TitleScene::Final()
{
	
}
