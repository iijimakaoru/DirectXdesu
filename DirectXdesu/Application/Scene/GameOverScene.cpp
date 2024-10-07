#include "GameOverScene.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

#include "PipelineManager.h"

GameOverScene::~GameOverScene()
{
	Final();
}

void GameOverScene::LoadResources()
{
	
}

void GameOverScene::Init()
{
	BaseScene::Init();

	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();
}

void GameOverScene::Update()
{
	camera->Update();

	// 次のシーンへ
	if (!sceneChange->GetIsEffect())
	{
		if (input->IsTrigger(DIK_SPACE) || input->GetPadButtonDown(XINPUT_GAMEPAD_A))
		{
			sceneChange->SceneChangeStart();
		}
	}

	if (sceneChange->GetIsChange())
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameOverScene::ObjDraw()
{
}

void GameOverScene::SpriteDraw()
{
	
}

void GameOverScene::Final()
{
}
