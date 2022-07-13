#include "KInput.h"
#include <cassert>

KInput::KInput(WNDCLASSEX w, HWND hwnd) {
	Init(w, hwnd);
}

void KInput::Init(WNDCLASSEX w, HWND hwnd) {
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

void KInput::Update() {
	// �L�[�{�[�h�̏��擾
	keyboad->Acquire();

	KeyInit();
	result = keyboad->GetDeviceState(sizeof(key), key);
}

void KInput::KeyInit() {
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
}

bool KInput::IsPush(int keyNum) {
	if (key[keyNum]) {
		return true;
	}
	return false;
}

bool KInput::IsNPush(int keyNum) {
	if (!key[keyNum]) {
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