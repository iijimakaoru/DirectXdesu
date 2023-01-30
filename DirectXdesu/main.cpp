#include <stdio.h>
#include <d3dcompiler.h>
#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#ifdef _DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")
#include "Sound.h"
#include <d3dx12.h>
#include "GameScence.h"
#include "ImguiManager.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	KWinApp::Init();

	KDirectXCommon::GetInstance()->Init();

	// �L�[�{�[�h����
	KInput::Init();

	std::unique_ptr<GameScence> gameScene = std::make_unique<GameScence>();

	ImguiManager imguiMane;
	imguiMane.Init();

	// �E�B���h�E�\��
	// �Q�[�����[�v
	while (true)
	{
		if (KWinApp::GetInstance()->ProcessMessage() || KInput::GetInstance()->IsPush(DIK_ESCAPE))
		{
			break;
		}

		// �X�V
		imguiMane.Begin();

		KInput::Update();

		gameScene->Update();

		imguiMane.End();

		// �`��J�n
		KDirectXCommon::GetInstance()->PreDraw();

		gameScene->Draw();

		imguiMane.Draw();

		KDirectXCommon::GetInstance()->PostDraw();
	}

	gameScene.release();

	return 0;
}