#include "KMyGame.h"

void KMyGame::Init()
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

	// 3Dオブジェクト静的初期化
	BillParticleManager::StaticInitialize(dx->GetDev(), win->GetWindowSizeW(), win->GetWindowSizeH());

	gameScene = std::make_unique<GameScence>();

	imguiMane.Init();
}

void KMyGame::Update()
{
	// 更新
	imguiMane.Begin();

	input->Update();

	gameScene->Update();

	imguiMane.End();
}

void KMyGame::Draw()
{
	// 描画開始
	dx->PreDraw();

	gameScene->Draw();

	imguiMane.Draw();

	dx->PostDraw();
}

void KMyGame::Final()
{
	gameScene.release();

	dx->Destroy();

	fbxLoader->Finalize();
}

bool KMyGame::IsEndRequest()
{
	if (win->ProcessMessage() || input->IsPush(DIK_ESCAPE))
	{
		return true;
	}

	return false;
}
