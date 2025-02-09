#pragma once
#pragma warning(push)
#pragma warning(disable: 4820)
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma warning(pop)
#include <wrl.h>
#include <Xinput.h>
#include "KWinApp.h"
#include "Vector2.h"
#include "Vector3.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

/**
 * @file KInput.h
 * @brief 入力
 * @author 飯島 薫
 */
 enum MouseBotton
{
	Left,
	Right,
	Wheel,
};

enum MouseBotton
{
	Left,
	Right,
	Wheel,
};

enum ControllerButton
{
	B = XINPUT_GAMEPAD_B,
	A = XINPUT_GAMEPAD_A,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	LT,
	RT
};

class KInput
{
public:

	// 初期化
	void Init();

	// 更新
	void Update();

	// キーボード
	// 押してるか
	bool IsPush(int keyNum);
	// 押したまま
	bool IsPress(int keyNum);
	// 押した瞬間
	bool IsTrigger(int keyNum);
	// 離した瞬間
	bool IsRelease(int keyNum);

	//マウス
	//マウスの状態を得る
	inline DIMOUSESTATE GetMouseState()
	{
		return GetInstance()->mouseState;
	}
	// 前回のマウスの状態を得る
	inline DIMOUSESTATE GetOldMouseState()
	{
		return GetInstance()->oldMouseState;
	}
	// マウスクリックされてるか
	bool GetMouseClick(MouseBotton bottonNum);
	// マウスクリックが離れた瞬間
	bool GetMouseClickRelease(MouseBotton bottonNum);
	// マウスクリックが押された瞬間
	bool GetMouseClickTrigger(MouseBotton bottonNum);
	// ホイール
	LONG GetMouseWheel();
	// マウスの位置取得
	KMyMath::Vector2 GetMousePos();
	// 前回のマウスの位置を取得
	KMyMath::Vector2 GetOldMousePos();
	// マウスの移動量を取得
	KMyMath::Vector3 GetMouseMove();

	// ゲームパッド
	//パッドに接続されてるか
	bool GetPadConnect();
	//パッドのボタンが押されているか
	bool GetPadButton(ControllerButton button);
	//パッドのボタンが離れた瞬間か
	bool GetPadButtonUp(ControllerButton button);
	//パッドのボタンが押された瞬間か
	bool GetPadButtonDown(ControllerButton button);
	//パッドの左スティック
	KMyMath::Vector2 GetPadLStick();
	//パッドの右スティック
	KMyMath::Vector2 GetPadRStick();
	//左トリガーを押し込んだ瞬間か
	bool GetLTriggerDown();
	//右トリガーを押し込んだ瞬間か
	bool GetRTriggerDown();
	//左スティックを上に倒した瞬間か
	bool GetLStickUp();
	//左スティックを下に倒した瞬間か
	bool GetLStickDown();

	/// <summary>
	/// 左スティックXのデッドゾーン設定
	/// </summary>
	/// <param name="incline"></param>
	/// <returns></returns>
	bool LStickTiltX(float incline);

	/// <summary>
	/// 左スティックYのデッドゾーン設定
	/// </summary>
	/// <param name="incline"></param>
	/// <returns></returns>
	bool LStickTiltY(float incline);

	// 左スティックの傾き
	KMyMath::Vector2 GetLStickInline();

	// 左スティックの角度取得
	float GetLStickAngle();

private:
	// 全キーの入力状態を所得
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

	// 入力生成
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;

	// キーボードデバイスの生成
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard = nullptr;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse = nullptr;
	DIMOUSESTATE mouseState = {};
	DIMOUSESTATE oldMouseState = {};
	KMyMath::Vector2 mousePos;
	KMyMath::Vector2 oldMousePos;

	// パッド
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
	KInput(const KInput&) = delete;
	const KInput& operator=(const KInput&) = delete;

	void InitInternal();
};

