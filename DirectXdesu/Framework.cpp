#include "Framework.h"

#include "Sprite.h"

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
}

void Framework::Final()
{
	dx->Destroy();

	fbxLoader->Finalize();
}

void Framework::Update()
{
	// 更新
	input->Update();
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
