#pragma once
#include "Camera.h"
#include "KObject3d.h"

/**
 * @file RailCamera.h
 * @brief レールカメラ
 * @author 飯島 薫
 */

class Player;

class RailCamera : public Camera
{
public:
	RailCamera() 
	{
		
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player_"></param>
	void Init(Player* player_, const KMyMath::Vector3& startPos);

	// 更新
	void Update();

	/// <summary>
	/// カメラ動かす
	/// </summary>
	void Move();

	/// <summary>
	/// 壊れた時のカメラワーク
	/// </summary>
	void Crash();

	/// <summary>
	/// 始まりのカメラワーク
	/// </summary>
	void Start();

	/// <summary>
	/// プレイヤーの角度と連動させる
	/// </summary>
	/// <param name="playersRot"></param>
	void SetRot();

	/// <summary>
	/// 前進フラグ
	/// </summary>
	/// <param name="isAdvance_"></param>
	void SetIsAdvance(bool isAdvance_);

	/// <summary>
	/// 死んだときに呼ぼう
	/// </summary>
	void CallCrash();

	/// <summary>
	/// ステージの始まりに呼ぼう
	/// </summary>
	void CallStart();

	/// <summary>
	/// カメラ速度セッター
	/// </summary>
	/// <returns></returns>
	static const float GetSpeed();

private:
	// 前進するか
	bool isAdvance = true;

	// 前進スピード
	static const float advanceSpeed;

	// 上下左右時のカメラの速さ(自機の何倍)
	static const float moveSpeedPlayerMagnification;

	// 移動上限
	KMyMath::Vector2 moveLimitMin = {};
	KMyMath::Vector2 moveLimitMax = {};

	// ゲームスタートで呼ぶ
	bool isStart = false;

	// プレイヤーが壊れたら呼ぶ
	bool isCrash = false;

	// ゲームクリアで呼ぶ
	bool isStageClear = false;

	// プレイヤーの型
	Player* player = nullptr;
};