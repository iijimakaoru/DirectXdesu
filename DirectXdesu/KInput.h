#pragma once
#include <dinput.h>
#define DIRECTINPUT_VERSION  0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <wrl.h>
#include <Windows.h>

using namespace Microsoft::WRL;

class KInput
{
public:
	KInput();
	void Init(HINSTANCE hInstance, HWND hwnd);
	void Update();
	void KeyInit();

	bool IsPush(int keyNum);
	bool IsNPush(int keyNum);
	bool IsPress(int keyNum);
	bool IsTriger(int keyNum);
	bool IsRelease(int keyNum);

private:
	// �S�L�[�̓��͏�Ԃ�����
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

	// ���͐���
	ComPtr<IDirectInput8> directInput = nullptr;

	// �L�[�{�[�h�f�o�C�X�̐���
	ComPtr<IDirectInputDevice8> keyboad = nullptr;

	HRESULT result;
};

