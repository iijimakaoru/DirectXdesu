#include "Input.h"
#include <cassert>

Input::Input(HRESULT& result, WNDCLASSEX w, HWND hwnd) {
	Init(result, w, hwnd);
}

void Input::Init(HRESULT &result, WNDCLASSEX w, HWND hwnd) {
	// ���͏�����
	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// �L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboad, NULL);
	assert(SUCCEEDED(result));

	// ���̓f�[�^�`���̃Z�b�g
	result = keyboad->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = keyboad->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update(HRESULT& result) {
	// �L�[�{�[�h�̏��擾
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