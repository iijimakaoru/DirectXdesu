#pragma once
#include <Windows.h>

class KWindow
{
public:
	KWindow();
	~KWindow();
	void Update();
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// ウィンドウサイズ
	const int window_width = 1280;
	const int window_height = 720;
	bool breakFlag = false;

	// ウィンドウクラスの設定
	WNDCLASSEX window{};
	// ウィンドウサイズ
	RECT rect = { 0,0,window_width,window_height };
	// メッセージ格納用構造体
	MSG msg{};

	HWND handle;
};

