#include "KWinApp.h"
#pragma comment(lib, "winmm.lib")
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT KWinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}
	// ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s�� 
	switch (msg) {
		// �E�B���h�E���j�����ꂽ 
	case WM_DESTROY:
		// OS�ɑ΂��āA�A�v���̏I����`���� 
		PostQuitMessage(0);
		return 0;
	}
	// �W���̃��b�Z�[�W�������s�� 
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void KWinApp::InitInternal()
{
	timeBeginPeriod(1);

	window.cbSize = sizeof(WNDCLASSEX);
	window.lpfnWndProc = (WNDPROC)WindowProc;
	window.lpszClassName = L"DirectX12";
	window.lpszMenuName = L"DirectX12";
	window.hInstance = GetModuleHandle(nullptr);
	window.hCursor = LoadCursor(NULL, IDC_ARROW);

	// �E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&window);

	// �����ŃT�C�Y�𒲐�����
	// �E�B���h�E�T�C�Y
	RECT rect = { 0,0,window_width,window_height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�쐬
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

	// �E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);
}

void KWinApp::Init()
{
	GetInstance()->InitInternal();
}

KWinApp* KWinApp::GetInstance()
{
	static KWinApp instance;
	return &instance;
}

void KWinApp::DeleteInstance()
{
	delete KWinApp::GetInstance();
}

int KWinApp::GetWindowSizeW()
{
	return GetInstance()->window_width;
}

int KWinApp::GetWindowSizeH()
{
	return GetInstance()->window_height;
}

bool KWinApp::ProcessMessage() {
	// ���b�Z�[�W�i�[�p�\����
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

HWND KWinApp::GetHWND()
{
	return GetInstance()->hwnd;
}

WNDCLASSEX KWinApp::GetWindow()
{
	return GetInstance()->window;
}
