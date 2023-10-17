#pragma once
#pragma warning(push)
#pragma warning(disable: 5039)
#pragma warning(disable: 4668)
#include <Windows.h>
#pragma warning(pop)

/**
 * @file KWinnApp.h
 * @brief ウィンドウ
 * @author 飯島 薫
 */

class KWinApp
{
private:
	KWinApp() = default;
	~KWinApp() = default;
	KWinApp(const KWinApp&) = delete;
	KWinApp& operator=(const KWinApp&) = delete;
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void InitInternal();

public:
	// 初期化
	static void Init();

	// インスタンス
	static KWinApp* GetInstance();

	// 削除
	static void DeleteInstance();

	// ウィンドウ横幅
	static int GetWindowSizeW();
	// ウィンドウ縦幅
	static int GetWindowSizeH();
	
	// プロセスメッセージ
	bool ProcessMessage();

	// 最終処理
	void Finalize();
	
	// ゲッター
	static HWND GetHWND();
	static WNDCLASSEX GetWindow();

private:
	// ウィンドウサイズ
	int window_width = 1280;
	int window_height = 720;

	// ウィンドウクラスの設定
	WNDCLASSEX window{};
	
	HWND hwnd;
};

