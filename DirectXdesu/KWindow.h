#pragma once
#include <Windows.h>

class KWindow
{
public:
	KWindow();
	~KWindow();
	void Update();
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// �E�B���h�E�T�C�Y
	const int window_width = 1280;
	const int window_height = 720;
	bool breakFlag = false;

	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX window{};
	// �E�B���h�E�T�C�Y
	RECT rect = { 0,0,window_width,window_height };
	// ���b�Z�[�W�i�[�p�\����
	MSG msg{};

	HWND handle;
};

