#include "Framework.h"
#include "Sprite.h"
#include "BaseScene.h"

void Framework::Init()
{
	// FBX初期化
	fbxLoader = FbxLoader::GetInstance();
	fbxLoader->Init();

	// ウィンドウ初期化
	win = KWinApp::GetInstance();
	win->Init();

	// DirectX初期化
	dx = KDirectXCommon::GetInstance();
	dx->Init();

	// Input初期化
	input = KInput::GetInstance();
	input->Init();

	Sprite::StaticInit();

	// シーンマネージャー生成
	sceneManager = SceneManager::GetInstance();

	// Imgui初期化
	imguiMane.Init();
}

void Framework::Final()
{
	// シーンマネージャーの解放
	sceneManager->Final();

	// シーンファクトリー解放
	delete sceneFactory;

	dx->Destroy();

	fbxLoader->Finalize();
}

void Framework::Update()
{
	// 更新
	input->Update();

	// Imgui更新はじめ
	imguiMane.Begin();

	// シーンマネージャーの更新
	sceneManager->Update();

	// Imgui更新終了
	imguiMane.End();
}

bool Framework::IsEndRwquest()
{
	if (win->ProcessMessage() || input->IsTrigger(DIK_ESCAPE))
	{
		return true;
	}

	return false;
}

void Framework::Run()
{
	Init();

	while (true)
	{
		// 終了リクエスト
		if (IsEndRwquest())
		{
			break;
		}

		// 更新
		Update();
		// 描画
		Draw();
	}
	// 終了
	Final();
}
