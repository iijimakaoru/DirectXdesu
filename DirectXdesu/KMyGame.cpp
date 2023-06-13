#include "KMyGame.h"

void KMyGame::Init()
{
	// FBX������
	fbxLoader = FbxLoader::GetInstance();
	fbxLoader->Init();

	// �E�B���h�E������
	win = KWinApp::GetInstance();
	win->Init();

	// DirectX������
	dx = KDirectXCommon::GetInstance();
	dx->Init();

	// Input������
	input = KInput::GetInstance();
	input->Init();

	// 3D�I�u�W�F�N�g�ÓI������
	BillParticleManager::StaticInitialize(dx->GetDev(), win->GetWindowSizeW(), win->GetWindowSizeH());

	gameScene = std::make_unique<GameScence>();

	imguiMane.Init();
}

void KMyGame::Update()
{
	// �X�V
	imguiMane.Begin();

	input->Update();

	gameScene->Update();

	imguiMane.End();
}

void KMyGame::Draw()
{
	// �`��J�n
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
