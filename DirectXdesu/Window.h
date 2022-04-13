#pragma once
#include <Windows.h>
#include "Std.h"

class Window
{
public:
	// �R���X�g���N�^
	Window();
	// �f�X�g���N�^
	~Window();
	// �E�B���h�E�n���h��
	HWND Get() const
	{
		return handle;
	}

private:
	static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void CreateWnd();

	WNDCLASSEX window;
	RECT rect;
	HWND handle;
};

