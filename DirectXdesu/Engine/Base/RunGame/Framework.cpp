#include "Framework.h"
#include "Sprite.h"
#include "PostEffect.h"
#include "BaseScene.h"

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

	// �ÓI������
	Sprite::StaticInit();
	PostEffect::StaticInit();

	// �V�[���}�l�[�W���[����
	sceneManager = SceneManager::GetInstance();

	// Imgui������
	imguiMane.Init();
}

void Framework::Final()
{
	// �V�[���}�l�[�W���[�̉��
	sceneManager->Final();

	// �V�[���t�@�N�g���[���
	delete sceneFactory;

	dx->Destroy();

	fbxLoader->Finalize();
}

void Framework::Update()
{
	// �X�V
	input->Update();

	// Imgui�X�V�͂���
	imguiMane.Begin();

	// �V�[���}�l�[�W���[�̍X�V
	sceneManager->Update();

	// Imgui�X�V�I��
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
