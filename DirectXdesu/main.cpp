#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#include "KSpriteCommon.h"
#include "Sprite.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	HRESULT result;
#pragma region 基盤初期化
#pragma region ウィンドウ
	KWinApp* win = nullptr;
	win = new KWinApp();
#pragma endregion
#pragma region DirectX初期化
	// KDirectCommon
	KDirectXCommon* dxCommon = nullptr;
	dxCommon = new KDirectXCommon();
	dxCommon->Init(win);
	// キーボード入力
	KInput* input = nullptr;
	input = new KInput();
	input->Init(win);
#pragma endregion
	SpriteCommon* spriteCommon = nullptr;

	spriteCommon = new SpriteCommon();
	spriteCommon->Init(dxCommon);

	spriteCommon->SpriteCommonLoadTexture(0, L"Resources/haikei.jpg");
#pragma endregion

#pragma region シーンの初期化
	Sprite* sprite = new Sprite();
	sprite->Init(spriteCommon);

	sprite->SpriteCreate();
	//sprite->SpriteTransferVertexBuffer();
#pragma endregion

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{
		// 更新
#pragma region 基盤の更新
		// ウィンドウメッセージ
		if (win->ProcessMessage() || input->IsPush(DIK_ESCAPE))
		{
			break;
		}

		// input更新
		input->Update();

		//sprite->SpriteUpdate();
#pragma endregion

#pragma region 描画
		// 描画開始
		dxCommon->PreDraw();

		sprite->SpriteDraw();
		
		// 描画終了
		dxCommon->PostDraw();
#pragma endregion
	}

#pragma region シーンの終了
	// 
	delete sprite;
#pragma endregion

#pragma region 基盤の終了
	// 
	delete spriteCommon;
	// 入力解放
	delete input;
	// DirectXCommon解放
	delete dxCommon;
	// WindowsAPI終了処理
	win->Finalize();
	// WindowsAPI解放
	delete win;
	win = nullptr;
#pragma endregion

	return 0;
}