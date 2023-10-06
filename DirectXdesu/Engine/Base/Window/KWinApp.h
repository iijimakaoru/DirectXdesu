#pragma once
#pragma warning(push)
#pragma warning(disable: 5039)
#pragma warning(disable: 4668)
#include <Windows.h>
#pragma warning(pop)

class KWinApp
{
private:
	KWinApp() = default;
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
	// ウィンドウサイズ
	int window_width = 1280;
	int window_height = 720;

	// ウィンドウクラスの設定
	WNDCLASSEX window{};
	
	HWND hwnd;
};

