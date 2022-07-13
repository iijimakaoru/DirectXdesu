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

	// �S�L�[�̓��͏�Ԃ�����
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

	// ���͐���
	IDirectInput8* directInput = nullptr;

	// �L�[�{�[�h�f�o�C�X�̐���
	IDirectInputDevice8* keyboad = nullptr;

	HRESULT result;
};

