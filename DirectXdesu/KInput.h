#pragma once
#include <dinput.h>
#define DIRECTINPUT_VERSION  0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
class KInput
{
public:
	KInput(WNDCLASSEX w, HWND hwnd);
	void Init(WNDCLASSEX w, HWND hwnd);
	void Update();
	void KeyInit();

	bool IsPush(int keyNum);
	bool IsNPush(int keyNum);
	bool IsPress(int keyNum);
	bool IsTriger(int keyNum);
	bool IsRelease(int keyNum);

	// 全キーの入力状態を所得
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

	// 入力生成
	IDirectInput8* directInput = nullptr;

	// キーボードデバイスの生成
	IDirectInputDevice8* keyboad = nullptr;

	HRESULT result;
};

