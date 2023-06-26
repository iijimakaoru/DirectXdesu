#include "KInput.h"
#include <cassert>
#include"KWinApp.h"
#include "MyMath.h"

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
	// ���͏�����
	result = DirectInput8Create(KWinApp::GetWindow().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// �L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));



	// ���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
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

	// �L�[�{�[�h�̏��擾
	instance->keyboard->Acquire();

	// �S�L�[���͏����擾
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

KMyMath::Vector3 KInput::GetMouseMove()
{
	return KMyMath::Vector3((float)GetInstance()->mouseState.lX, (float)GetInstance()->mouseState.lY, (float)GetInstance()->mouseState.lZ);
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

bool KInput::LStickTiltX(float incline)
{
	//���ɓ|�������̔���
	if (incline > 0)
	{
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�����true��Ԃ�
		if (xInputState.Gamepad.sThumbLX >= incline)
		{
			return true;
		}
	}
	//��ɓ|�������̔���
	else if (incline < 0)
	{
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�������true��Ԃ�
		if (xInputState.Gamepad.sThumbLX <= incline)
		{
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool KInput::LStickTiltY(float incline)
{
	//���ɓ|�������̔���
	if (incline > 0) 
	{
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�����true��Ԃ�
		if (xInputState.Gamepad.sThumbLY >= incline) 
		{
			return true;
		}
	}
	//��ɓ|�������̔���
	else if (incline < 0) 
	{
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�������true��Ԃ�
		if (xInputState.Gamepad.sThumbLY <= incline) 
		{
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

KMyMath::Vector2 KInput::GetLStickInline()
{
	return KMyMath::Vector2((float)xInputState.Gamepad.sThumbLX / 1000, (float)xInputState.Gamepad.sThumbLY / 1000);
}

float KInput::GetLStickAngle()
{
	KMyMath::Vector2 tilt = GetLStickInline();
	float x = tilt.x;
	float y = tilt.y;

	float radians = atan2f(y, x) * 360 / MyMathUtility::PIDouble;

	// �}�C�i�X�������ꍇ�̏C��
	if (radians < 0)
	{
		radians += 360;
	}

	return radians;
}
