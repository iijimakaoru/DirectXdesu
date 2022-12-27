#include "KInput.h"
#include <cassert>

void KInput::Init(KWinApp* win) {
	assert(win);

	KInput::GetInstance()->win = win;

	// ���͏�����
	KInput::GetInstance()->result = DirectInput8Create(win->GetWindow().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&KInput::GetInstance()->directInput, nullptr);
	assert(SUCCEEDED(KInput::GetInstance()->result));

	// �L�[�{�[�h�f�o�C�X�̐���
	KInput::GetInstance()->result = KInput::GetInstance()->directInput->CreateDevice(GUID_SysKeyboard, &KInput::GetInstance()->keyboad, NULL);
	assert(SUCCEEDED(KInput::GetInstance()->result));

	// ���̓f�[�^�`���̃Z�b�g
	KInput::GetInstance()->result = KInput::GetInstance()->keyboad->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(KInput::GetInstance()->result));

	// �r�����䃌�x���̃Z�b�g
	KInput::GetInstance()->result = KInput::GetInstance()->keyboad->SetCooperativeLevel(win->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(KInput::GetInstance()->result));
}

void KInput::Update() {
	// �L�[�{�[�h�̏��擾
	KInput::GetInstance()->keyboad->Acquire();
	// ��t���[���O�̓��͂̕ۑ�
	KeyInit();
	// �S�L�[���͏����擾
	KInput::GetInstance()->result = KInput::GetInstance()->keyboad->GetDeviceState(sizeof(KInput::GetInstance()->key), KInput::GetInstance()->key);
}

void KInput::KeyInit() {
	for (int i = 0; i < 256; i++)
	{
		KInput::GetInstance()->oldkey[i] = KInput::GetInstance()->key[i];
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

KInput* KInput::GetInstance()
{
	static KInput instance;
	return &instance;
}
