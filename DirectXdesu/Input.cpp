#include "Input.h"
#include <cassert>

Input::Input(HRESULT& result, WNDCLASSEX w, HWND hwnd) {
	Init(result, w, hwnd);
}

void Input::Init(HRESULT &result, WNDCLASSEX w, HWND hwnd) {
	// 入力初期化
	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboad, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboad->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboad->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update(HRESULT& result) {
	// キーボードの情報取得
	keyboad->Acquire();

	KeyInit();
	result = keyboad->GetDeviceState(sizeof(key), key);
}

void Input::KeyInit() {
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
}

bool Input::IsPush(int keyNum) {
	if (key[keyNum]) {
		return true;
	}
	return false;
}

bool Input::IsNPush(int keyNum) {
	if (!key[keyNum]) {
		return true;
	}
	return false;
}

bool Input::IsPress(int keyNum) {
	if (key[keyNum] && oldkey[keyNum]) {
		return true;
	}
	return false;
}

bool Input::IsTriger(int keyNum) {
	if (key[keyNum] && !oldkey[keyNum]) {
		return true;
	}
	return false;
}

bool Input::IsRelease(int keyNum) {
	if (!key[keyNum] && oldkey[keyNum]) {
		return true;
	}
	return false;
}