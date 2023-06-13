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

#include "FbxLoader.h"

#include "KMyGame.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	KMyGame game;
	game.Init();

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{
		if (game.IsEndRequest())
		{
			break;
		}

		// 毎フレーム更新
		game.Update();

		// 描画開始
		game.Draw();
	}

	// 終了
	game.Final();

	return 0;
}