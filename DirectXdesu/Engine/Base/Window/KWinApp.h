#pragma once
#include <Windows.h>

class KWinApp
{
private:
	KWinApp() {};
	~KWinApp() = default;
	KWinApp(const KWinApp&) = delete;
	KWinApp& operator=(const KWinApp&) = delete;
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void InitInternal();

public:
	static void Init();
	static KWinApp* GetInstance();
	static void DeleteInstance();

	static int GetWindowSizeW();
	static int GetWindowSizeH();
	
	bool ProcessMessage();
	void Finalize();
	
	static HWND GetHWND();
	static WNDCLASSEX GetWindow();

private:
	// �E�B���h�E�T�C�Y
	int window_width = 1280;
	int window_height = 720;

	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX window{};
	
	HWND hwnd;
};

