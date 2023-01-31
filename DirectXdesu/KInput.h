#pragma once
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <wrl.h>
#include <Windows.h>
#include "KWinApp.h"
#include <Xinput.h>
#include "Vector2.h"

#pragma comment(lib, "xinput.lib")

using namespace Microsoft::WRL;

class KInput
{
public:
	static void Init();
	static void Update();

	bool IsPush(int keyNum);
	bool IsPress(int keyNum);
	bool IsTriger(int keyNum);
	bool IsRelease(int keyNum);

	//�p�b�h�ɐڑ�����Ă邩
	bool GetPadConnect();

	//�p�b�h�̃{�^����������Ă��邩
	bool GetPadButton(UINT button);

	//�p�b�h�̃{�^�������ꂽ�u�Ԃ�
	bool GetPadButtonUp(UINT button);

	//�p�b�h�̃{�^���������ꂽ�u�Ԃ�
	bool GetPadButtonDown(UINT button);

	//�p�b�h�̍��X�e�B�b�N
	Vector2 GetPadLStick();

	//�p�b�h�̉E�X�e�B�b�N
	Vector2 GetPadRStick();

	//���g���K�[���������񂾏u�Ԃ�
	bool GetLTriggerDown();

	//�E�g���K�[���������񂾏u�Ԃ�
	bool GetRTriggerDown();

	//���X�e�B�b�N����ɓ|�����u�Ԃ�
	bool GetLStickUp();

	//���X�e�B�b�N�����ɓ|�����u�Ԃ�
	bool GetLStickDown();

private:
	// �S�L�[�̓��͏�Ԃ�����
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

	// ���͐���
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;

	// �L�[�{�[�h�f�o�C�X�̐���
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard = nullptr;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse = nullptr;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 oldMouseState = {};
	Vector2 mousePos;
	Vector2 oldMousePos;

	// �p�b�h
	XINPUT_STATE xInputState;
	XINPUT_STATE oldXInputState;
	bool isConnectPad = false;

	HRESULT result;

public:
	static KInput* GetInstance();
	static void DeleteInstance();

private:
	KInput() {};
	~KInput() = default;
	KInput(const KInput&) = default;
	const KInput& operator=(const KInput&) = delete;

	void InitInternal();
};

