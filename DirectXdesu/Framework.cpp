#include "Framework.h"

void Framework::Init()
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
	// �X�V
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
		// �I�����N�G�X�g
		if (IsEndRwquest())
		{
			break;
		}

		// �X�V
		Update();
		// �`��
		Draw();
	}
	// �I��
	Final();
}
