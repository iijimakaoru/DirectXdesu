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
	// �E�B���h�E���j�����ꂽ�Ƃ�
	if (msg == WM_DESTROY)
	{
		// os�ɑ΂��ăA�v���P�[�V�����I����`����
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::CreateWnd()
{
	window.cbClsExtra = 0;

	// WNDCLASSEX�̃T�C�Y���w��
	window.cbSize = sizeof(WNDCLASSEX);

	window.cbWndExtra = 0;

	// �E�B���h�E�̔w�i�F�̎w��
	window.hbrBackground = CreateSolidBrush(0xaaaaaa);

	// �}�E�X�J�[�\���̎w��
	window.hCursor = LoadCursor(NULL, IDC_ARROW);

	//�g�p����A�C�R���̎w��
	window.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	//�g�p����A�C�R���̎w��
	window.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// �C���X�^���X�n���h���̎w��
	window.hInstance = GetModuleHandle(0);

	// �E�B���h�E�v���V�[�W���̎w��
	window.lpfnWndProc = (WNDPROC)WindowProcedure;

	// �E�B���h�E�̖��O
	window.lpszClassName = _T("DirectX12");

	// �E�B���h�E���j���[�̖��O
	window.lpszMenuName = _T("DirectX12");

	// �E�B���h�E�̏��������̎w��
	window.style = CS_HREDRAW | CS_VREDRAW;

	// �E�B���h�E�̓o�^
	RegisterClassEx(&window);

	// �E�B���h�E�T�C�Y�̎w��
	// �c��
	rect.bottom = WINDOW_HEIGHT;
	rect.left = 0;
	// ����
	rect.right = WINDOW_WIDTH;
	rect.top = 0;

	// �T�C�Y�̕␳
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�쐬
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
