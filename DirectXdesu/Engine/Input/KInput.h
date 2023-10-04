#pragma once
#include <dinput.h>
#include <wrl.h>
#include <Xinput.h>
#include "KWinApp.h"
#include "Vector2.h"
#include "Vector3.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

class KInput
{
public:
	 enum MouseBotton
	{
		Left,
		Right,
		Wheel,
	};

public:
	static void Init();
	static void Update();

	// �L�[�{�[�h
	// �����Ă邩
	bool IsPush(int keyNum);
	// �������܂�
	bool IsPress(int keyNum);
	// �������u��
	bool IsTrigger(int keyNum);
	// �������u��
	bool IsRelease(int keyNum);

	//�}�E�X
	//�}�E�X�̏�Ԃ𓾂�
	static inline DIMOUSESTATE2 GetMouseState()
	{
		return GetInstance()->mouseState;
	}
	// �O��̃}�E�X�̏�Ԃ𓾂�
	static inline DIMOUSESTATE2 GetOldMouseState()
	{
		return GetInstance()->oldMouseState;
	}
	// �}�E�X�N���b�N����Ă邩
	static bool GetMouseClick(int bottonNum);
	// �}�E�X�N���b�N�����ꂽ�u��
	static bool GetMouseClickRelease(int bottonNum);
	// �}�E�X�N���b�N�������ꂽ�u��
	static bool GetMouseClickTrigger(int bottonNum);
	// �z�C�[��
	static LONG GetMouseWheel();
	// �}�E�X�̈ʒu�擾
	static KMyMath::Vector2 GetMousePos();
	// �O��̃}�E�X�̈ʒu���擾
	static KMyMath::Vector2 GetOldMousePos();
	// �}�E�X�̈ړ��ʂ��擾
	static KMyMath::Vector3 GetMouseMove();

	// �Q�[���p�b�h
	//�p�b�h�ɐڑ�����Ă邩
	bool GetPadConnect();
	//�p�b�h�̃{�^����������Ă��邩
	bool GetPadButton(UINT button);
	//�p�b�h�̃{�^�������ꂽ�u�Ԃ�
	bool GetPadButtonUp(UINT button);
	//�p�b�h�̃{�^���������ꂽ�u�Ԃ�
	bool GetPadButtonDown(UINT button);
	//�p�b�h�̍��X�e�B�b�N
	KMyMath::Vector2 GetPadLStick();
	//�p�b�h�̉E�X�e�B�b�N
	KMyMath::Vector2 GetPadRStick();
	//���g���K�[���������񂾏u�Ԃ�
	bool GetLTriggerDown();
	//�E�g���K�[���������񂾏u�Ԃ�
	bool GetRTriggerDown();
	//���X�e�B�b�N����ɓ|�����u�Ԃ�
	bool GetLStickUp();
	//���X�e�B�b�N�����ɓ|�����u�Ԃ�
	bool GetLStickDown();

	/// <summary>
	/// ���X�e�B�b�NX�̃f�b�h�]�[���ݒ�
	/// </summary>
	/// <param name="incline"></param>
	/// <returns></returns>
	bool LStickTiltX(float incline);

	/// <summary>
	/// ���X�e�B�b�NY�̃f�b�h�]�[���ݒ�
	/// </summary>
	/// <param name="incline"></param>
	/// <returns></returns>
	bool LStickTiltY(float incline);

	// ���X�e�B�b�N�̌X��
	KMyMath::Vector2 GetLStickInline();

	// ���X�e�B�b�N�̊p�x�擾
	float GetLStickAngle();

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
	KMyMath::Vector2 mousePos;
	KMyMath::Vector2 oldMousePos;

	// �p�b�h
	XINPUT_STATE xInputState;
	XINPUT_STATE oldXInputState;
	bool isConnectPad = false;

	HRESULT result;

public:
	static KInput* GetInstance();
	static void DeleteInstance();

private:
	KInput() = default;
	~KInput() = default;
	KInput(const KInput&) = default;
	const KInput& operator=(const KInput&) = delete;

	void InitInternal();
};

