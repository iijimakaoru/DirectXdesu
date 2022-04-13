#pragma once
#include <Windows.h>
#include "Std.h"

class Window
{
public:
	// コンストラクタ
	Window();
	// デストラクタ
	~Window();
	// ウィンドウハンドル
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

