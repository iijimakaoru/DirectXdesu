#include <stdio.h>
#include <d3dcompiler.h>
#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#include "KDepth.h"
#include "KTexture.h"
#include "KObject3d.h"
#include "ViewProjection.h"
#include "KModel.h"
#include "KShader.h"
#ifdef _DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")
#include "Sound.h"
#include <d3dx12.h>
#include "ConstBuffer.h"
#include "PipelineSet.h"
#include "Vector4.h"
#include "Sprite.h"
#include "DebugText.h"

#include <array>

#include"Player.h"
#include"Stage.h"
#include"Boss.h"
#include "DebugCamera.h"
#include "Camera.h"
#include "MyMath.h"
#include "ParticleManager.h"
#include "GameScence.h"

#include "KGPlin.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	KWinApp::Init();

	KDirectXCommon::GetInstance()->Init();

	// �L�[�{�[�h����
	KInput::Init();

	std::unique_ptr<GameScence> gameScene = std::make_unique<GameScence>();

	// �E�B���h�E�\��
	// �Q�[�����[�v
	while (true)
	{
		if (KWinApp::GetInstance()->ProcessMessage() || KInput::GetInstance()->IsPush(DIK_ESCAPE))
		{
			break;
		}

		// �X�V
		KInput::Update();

		gameScene->Update();

		// �`��J�n
		KDirectXCommon::GetInstance()->PreDraw();

		gameScene->Draw();

		KDirectXCommon::GetInstance()->PostDraw();
	}

#pragma region ��Ղ̏I��
	/*sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);*/
#pragma endregion

	return 0;
}