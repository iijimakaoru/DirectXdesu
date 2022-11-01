#pragma once
#include <Windows.h>

class KWinApp
{
public:
	KWinApp();
	~KWinApp();
	bool ProcessMessage();
	void Finalize();
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	HWND GetHWND() { return hwnd; }
	WNDCLASSEX GetWindow() { return window; }

	// ウィンドウサイズ
	static const int window_width = 1280;
	static const int window_height = 720;

private:
	// ウィンドウクラスの設定
	WNDCLASSEX window{};
	// ウィンドウサイズ
	RECT rect = { 0,0,window_width,window_height };
	
	HWND hwnd;
};

