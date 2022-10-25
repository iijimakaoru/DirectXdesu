#pragma once
#include <dinput.h>
#define DIRECTINPUT_VERSION  0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <wrl.h>
#include <Windows.h>
#include "KWinApp.h"

using namespace Microsoft::WRL;

class KInput
{
public:
	KInput();
	void Init(KWinApp* win);
	void Update();
	void KeyInit();

	bool IsPush(int keyNum);
	bool IsPress(int keyNum);
	bool IsTriger(int keyNum);
	bool IsRelease(int keyNum);

private:
	// 全キーの入力状態を所得
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

	// 入力生成
	ComPtr<IDirectInput8> directInput = nullptr;

	// キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keyboad = nullptr;

	KWinApp* win = nullptr;

	HRESULT result;
};

