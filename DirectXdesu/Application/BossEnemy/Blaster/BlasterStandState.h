#pragma once
#include "ActState.h"
#include "MyMath.h"

class BlasterStandState : public ActState
{
public:
	// コンストラクタ
	BlasterStandState();

	// 更新
	void Update() override;

private:
	float angle = 0;
	float startTimer = 0;
	float startTime = 0;
};

