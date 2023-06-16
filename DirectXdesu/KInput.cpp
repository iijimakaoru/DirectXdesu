#include "KInput.h"
#include <cassert>
#include"KWinApp.h"

KInput* KInput::GetInstance()
{
	static KInput instance;
	return &instance;
}

void KInput::DeleteInstance()
{
	delete KInput::GetInstance();
}

void KInput::Init() 
{
	GetInstance()->InitInternal();
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
	KInput* instance = GetInstance();

	// キーボードの情報取得
	instance->keyboard->Acquire();

	// 全キー入力情報を取得
	for (int i = 0; i < 256; i++)
	{
		instance->oldkey[i] = instance->key[i];
	}
	instance->keyboard->GetDeviceState(sizeof(key), instance->key);

	instance->oldXInputState = instance->xInputState;
	DWORD dresult = XInputGetState(0, &instance->xInputState);
	if (dresult == ERROR_SUCCESS) {
		instance->isConnectPad = true;
	}
	else {
		instance->isConnectPad = false;
	}

	if ((instance->xInputState.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		instance->xInputState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(instance->xInputState.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			instance->xInputState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		instance->xInputState.Gamepad.sThumbLX = 0;
		instance->xInputState.Gamepad.sThumbLY = 0;
	}

	if ((instance->xInputState.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		instance->xInputState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(instance->xInputState.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			instance->xInputState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		instance->xInputState.Gamepad.sThumbRX = 0;
		instance->xInputState.Gamepad.sThumbRY = 0;
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

bool KInput::IsTrigger(int keyNum) {
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

bool KInput::GetMouseClick(int bottonNum)
{
	return (GetInstance()->mouseState.rgbButtons[bottonNum] & 0x80) != 0;
}

bool KInput::GetMouseClickRelease(int bottonNum)
{
	return (GetInstance()->mouseState.rgbButtons[bottonNum] & 0x80) == 0 &&
		(GetInstance()->oldMouseState.rgbButtons[bottonNum] & 0x80) != 0;
}

bool KInput::GetMouseClickTrigger(int bottonNum)
{
	return (GetInstance()->mouseState.rgbButtons[bottonNum] & 0x80) != 0 &&
		(GetInstance()->oldMouseState.rgbButtons[bottonNum] & 0x80) == 0;
}

LONG KInput::GetMouseWheel()
{
	return GetInstance()->mouseState.lZ;
}

KMyMath::Vector2 KInput::GetMousePos()
{
	return GetInstance()->mousePos;
}

KMyMath::Vector2 KInput::GetOldMousePos()
{
	return GetInstance()->oldMousePos;
}

Vector3 KInput::GetMouseMove()
{
	return Vector3((float)GetInstance()->mouseState.lX, (float)GetInstance()->mouseState.lY, (float)GetInstance()->mouseState.lZ);
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

KMyMath::Vector2 KInput::GetPadLStick()
{
	SHORT x = xInputState.Gamepad.sThumbLX;
	SHORT y = xInputState.Gamepad.sThumbLY;

	return KMyMath::Vector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
}

KMyMath::Vector2 KInput::GetPadRStick()
{
	SHORT x = xInputState.Gamepad.sThumbRX;
	SHORT y = xInputState.Gamepad.sThumbRY;

	return KMyMath::Vector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
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