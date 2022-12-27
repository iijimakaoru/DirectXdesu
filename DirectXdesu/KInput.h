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
	static void Init(KWinApp* win);
	static void Update();
	static void KeyInit();

	bool IsPush(int keyNum);
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

	KWinApp* win = nullptr;

	HRESULT result;

public:
	static KInput* GetInstance();

private:
	KInput() = default;
	~KInput() = default;
	KInput(const KInput&) = default;
	const KInput& operator=(const KInput&) = delete;
};

