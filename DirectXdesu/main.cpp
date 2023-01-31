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
#include "ParticleManager.h"
#include "SpriteCommon.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	KWinApp* win = KWinApp::GetInstance();
	win->Init();

	KDirectXCommon* dx = KDirectXCommon::GetInstance();
	dx->Init();

	// キーボード入力
	KInput* input = KInput::GetInstance();
	input->Init();

	AhoSpriteCommon* spriteCommon = AhoSpriteCommon::GetInstance();

	std::unique_ptr<GameScence> gameScene = std::make_unique<GameScence>();

	ImguiManager imguiMane;
	imguiMane.Init();

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{
		if (KWinApp::GetInstance()->ProcessMessage() || KInput::GetInstance()->IsPush(DIK_ESCAPE))
		{
			//// 変数名はサンプルコードに合わせた。
			//ComPtr<ID3D12DebugDevice> pDebugDevice = NULL;
			//KDirectXCommon::GetInstance()->GetDev()->QueryInterface(pDebugDevice.GetAddressOf());
			//KDirectXCommon::GetInstance()->GetDev()->Release();
			//pDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
			//pDebugDevice->Release();
			break;
		}

		// 更新
		imguiMane.Begin();

		input->Update();

		gameScene->Update();

		imguiMane.End();

		// 描画開始
		KDirectXCommon::GetInstance()->PreDraw();

		gameScene->Draw();

		imguiMane.Draw();

		KDirectXCommon::GetInstance()->PostDraw();
	}

	gameScene.release();

	return 0;
}