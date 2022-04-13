#include "Window.h"

Window::Window()
{
	CreateWnd();
}

Window::~Window()
{
	UnregisterClass(window.lpszClassName, window.hInstance);
}

LRESULT CALLBACK Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ウィンドウが破棄されたとき
	if (msg == WM_DESTROY)
	{
		// osに対してアプリケーション終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::CreateWnd()
{
	window.cbClsExtra = 0;

	// WNDCLASSEXのサイズを指定
	window.cbSize = sizeof(WNDCLASSEX);

	window.cbWndExtra = 0;

	// ウィンドウの背景色の指定
	window.hbrBackground = CreateSolidBrush(0xaaaaaa);

	// マウスカーソルの指定
	window.hCursor = LoadCursor(NULL, IDC_ARROW);

	//使用するアイコンの指定
	window.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	//使用するアイコンの指定
	window.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// インスタンスハンドルの指定
	window.hInstance = GetModuleHandle(0);

	// ウィンドウプロシージャの指定
	window.lpfnWndProc = (WNDPROC)WindowProcedure;

	// ウィンドウの名前
	window.lpszClassName = _T("DirectX12");

	// ウィンドウメニューの名前
	window.lpszMenuName = _T("DirectX12");

	// ウィンドウの書き直しの指定
	window.style = CS_HREDRAW | CS_VREDRAW;

	// ウィンドウの登録
	RegisterClassEx(&window);

	// ウィンドウサイズの指定
	// 縦幅
	rect.bottom = WINDOW_HEIGHT;
	rect.left = 0;
	// 横幅
	rect.right = WINDOW_WIDTH;
	rect.top = 0;

	// サイズの補正
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウ作成
	handle = CreateWindow(
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
}
