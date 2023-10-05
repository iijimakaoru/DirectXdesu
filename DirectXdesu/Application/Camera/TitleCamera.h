#pragma once
#include "Camera.h"

class TitleCamera : public Camera
{
public:
	TitleCamera()
	{
		
	}

	// 初期化
	void Init();

	// 更新
	void Update();

	// タイトルカメラワーク
	void RoundCamera();

	// 回転スタート
	void StartRound();

	// 出撃カメラワーク
	void SortieCamera();

	// 出撃スタート
	void StartSortie();

private:
	// 角度
	float rotAngle = 0;

	// 回転カメラフラグ
	bool isRound = false;

	// 出撃カメラフラグ
	bool isSortie = false;

	// 出撃カメラ演出
	uint32_t sortiePhase = 0;
	float sortiePhaseTimer = 0;
	float sortiePhaseTime = 0;

	// 今の角度
	float nowAngle = 0;
};

