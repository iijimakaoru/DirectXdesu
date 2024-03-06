#pragma once
#include "Camera.h"

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
	void Update(bool isStart_, bool isBossMovie_, bool isClearMovie_);

	/// <summary>
	/// カメラ動かす
	/// </summary>
	void Move();

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
	/// スタートカメラワークが終わった時の処理
	/// </summary>
	void EndStart();

	/// <summary>
	/// カメラ速度ゲッター
	/// </summary>
	/// <returns></returns>
	static const float GetSpeed();

private:
	/// <summary>
	/// 壊れた時のカメラワーク
	/// </summary>
	void Crash();

	/// <summary>
	/// プレイヤーの角度と連動させる
	/// </summary>
	/// <param name="playersRot"></param>
	void SetRot();

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

	// プレイヤーが壊れたら呼ぶ
	bool isCrash = false;

	// ゲームクリアで呼ぶ
	bool isStageClear = false;

	// プレイヤーの型
	Player* player = nullptr;

	// 角度
	float rotAngle = 0;

	// スタート位置
	KMyMath::Vector3 startPos = { 0,0,0 };
};