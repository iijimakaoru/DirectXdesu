#include "KInput.h"
#include <cassert>
#include"KWinApp.h"

KInput* KInput::GetInstance()
{
	static KInput instance;
	return &instance;
}

void KInput::Init() 
{
	InitInternal();
}

void KInput::InitInternal()
{
	// 入力初期化
	result = DirectInput8Create(KWinApp::GetWindow().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(KWinApp::GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));
	DWORD dresult;
	dresult = XInputGetState(0, &xInputState);

	if (result == ERROR_SUCCESS)
	{
		isConnectPad = true;
	}
	else
	{
		isConnectPad = false;
	}
}

void KInput::Update() 
{
	// キーボードの情報取得
	keyboard->Acquire();

	// 全キー入力情報を取得
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
	keyboard->GetDeviceState(sizeof(key), key);

	oldXInputState = xInputState;
	DWORD dresult = XInputGetState(0, &xInputState);
	if (dresult == ERROR_SUCCESS) {
		isConnectPad = true;
	}
	else {
		isConnectPad = false;
	}

	if ((xInputState.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		xInputState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(xInputState.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			xInputState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		xInputState.Gamepad.sThumbLX = 0;
		xInputState.Gamepad.sThumbLY = 0;
	}

	if ((xInputState.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		xInputState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(xInputState.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			xInputState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		xInputState.Gamepad.sThumbRX = 0;
		xInputState.Gamepad.sThumbRY = 0;
	}
}

bool KInput::IsPush(int keyNum) {
	if (key[keyNum]) {
		return true;
	}
	return false;
}

bool KInput::IsPress(int keyNum) {
	if (key[keyNum] && oldkey[keyNum]) {
		return true;
	}
	return false;
}

bool KInput::IsTriger(int keyNum) {
	if (key[keyNum] && !oldkey[keyNum]) {
		return true;
	}
	return false;
}

bool KInput::IsRelease(int keyNum) {
	if (!key[keyNum] && oldkey[keyNum]) {
		return true;
	}
	return false;
}

bool KInput::GetPadConnect()
{
	return isConnectPad;
}

bool KInput::GetPadButton(UINT button)
{
	return xInputState.Gamepad.wButtons == button;
}

bool KInput::GetPadButtonUp(UINT button)
{
	return xInputState.Gamepad.wButtons != button && oldXInputState.Gamepad.wButtons == button;
}

bool KInput::GetPadButtonDown(UINT button)
{
	return xInputState.Gamepad.wButtons == button && oldXInputState.Gamepad.wButtons != button;
}

Vector2 KInput::GetPadLStick()
{
	SHORT x = xInputState.Gamepad.sThumbLX;
	SHORT y = xInputState.Gamepad.sThumbLY;

	return Vector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
}

Vector2 KInput::GetPadRStick()
{
	SHORT x = xInputState.Gamepad.sThumbRX;
	SHORT y = xInputState.Gamepad.sThumbRY;

	return Vector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
}

bool KInput::GetLTriggerDown()
{
	if (oldXInputState.Gamepad.bLeftTrigger < 128 && xInputState.Gamepad.bLeftTrigger >= 128)
	{
		return true;
	}
	return false;
}

bool KInput::GetRTriggerDown()
{
	if (oldXInputState.Gamepad.bRightTrigger < 128 && xInputState.Gamepad.bRightTrigger >= 128)
	{
		return true;
	}
	return false;
}

bool KInput::GetLStickUp()
{
	if (oldXInputState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		xInputState.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return true;
	}
	return false;
}

bool KInput::GetLStickDown()
{
	if (oldXInputState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		xInputState.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return true;
	}
	return false;
}
