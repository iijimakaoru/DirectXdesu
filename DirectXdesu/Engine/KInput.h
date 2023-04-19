#pragma once
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <wrl.h>
#include <Windows.h>
#include "KWinApp.h"
#include <Xinput.h>
#include "Vector2.h"
#include "Vector3.h"

#pragma comment(lib, "xinput.lib")

using namespace Microsoft::WRL;

class KInput
{
public:
	static enum MouseBotton
	{
		Left,
		Right,
		Wheel,
	};

public:
	static void Init();
	static void Update();

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
	static inline DIMOUSESTATE2 GetMouseState()
	{
		return GetInstance()->mouseState;
	}
	// 前回のマウスの状態を得る
	static inline DIMOUSESTATE2 GetOldMouseState()
	{
		return GetInstance()->oldMouseState;
	}
	// マウスクリックされてるか
	static bool GetMouseClick(int bottonNum);
	// マウスクリックが離れた瞬間
	static bool GetMouseClickRelease(int bottonNum);
	// マウスクリックが押された瞬間
	static bool GetMouseClickTrigger(int bottonNum);
	// ホイール
	static LONG GetMouseWheel();
	// マウスの位置取得
	static Vector2 GetMousePos();
	// 前回のマウスの位置を取得
	static Vector2 GetOldMousePos();
	// マウスの移動量を取得
	static Vector3 GetMouseMove();

	// ゲームパッド
	//パッドに接続されてるか
	bool GetPadConnect();
	//パッドのボタンが押されているか
	bool GetPadButton(UINT button);
	//パッドのボタンが離れた瞬間か
	bool GetPadButtonUp(UINT button);
	//パッドのボタンが押された瞬間か
	bool GetPadButtonDown(UINT button);
	//パッドの左スティック
	Vector2 GetPadLStick();
	//パッドの右スティック
	Vector2 GetPadRStick();
	//左トリガーを押し込んだ瞬間か
	bool GetLTriggerDown();
	//右トリガーを押し込んだ瞬間か
	bool GetRTriggerDown();
	//左スティックを上に倒した瞬間か
	bool GetLStickUp();
	//左スティックを下に倒した瞬間か
	bool GetLStickDown();

private:
	// 全キーの入力状態を所得
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

	// 入力生成
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;

	// キーボードデバイスの生成
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard = nullptr;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse = nullptr;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 oldMouseState = {};
	Vector2 mousePos;
	Vector2 oldMousePos;

	// パッド
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

