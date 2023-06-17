#include "Framework.h"

#include "Sprite.h"

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

	Sprite::StaticInit();
}

void Framework::Final()
{
	dx->Destroy();

	fbxLoader->Finalize();
}

void Framework::Update()
{
	// �X�V
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
