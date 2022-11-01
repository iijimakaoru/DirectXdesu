#include "KWinApp.h"
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

LRESULT KWinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// メッセージに応じてゲーム固有の処理を行う 
	switch (msg) {
		// ウィンドウが破棄された 
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える 
		PostQuitMessage(0);
		return 0;
	}
	// 標準のメッセージ処理を行う 
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool KWinApp::ProcessMessage() {
	// メッセージ格納用構造体
	MSG msg{};
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

void KWinApp::Finalize()
{
	UnregisterClass(window.lpszClassName, window.hInstance);
}

KWinApp::KWinApp() {
	timeBeginPeriod(1);

	window.cbSize = sizeof(WNDCLASSEX);
	window.lpfnWndProc = (WNDPROC)WindowProc;
	window.lpszClassName = L"DirectX12";
	window.lpszMenuName = L"DirectX12";
	window.hInstance = GetModuleHandle(nullptr);
	window.hCursor = LoadCursor(NULL, IDC_ARROW);

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&window);

	// 自分でサイズを調整する
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウ作成
	hwnd = CreateWindow(
		window.lpszClassName,
		window.lpszMenuName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		nullptr,
		nullptr,
		window.hInstance,
		nullptr);

	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);
}

KWinApp::~KWinApp() {

}