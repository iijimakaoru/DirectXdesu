#include "Framework.h"

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

	gameScene = std::make_unique<GameScence>();

	imguiMane.Init();
}

void Framework::Final()
{
	gameScene.release();

	dx->Destroy();

	fbxLoader->Finalize();
}

void Framework::Update()
{
	// 更新
	imguiMane.Begin();

	input->Update();

	gameScene->Update();

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
