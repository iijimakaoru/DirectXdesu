#pragma once
#include "ActState.h"
class BlasterAimState : public ActState
{
public:
	// コンストラクタ
	BlasterAimState();
	// 更新
	void Update() override;

	void BulletShot();

private:
	float angle = 0;
	float startTimer = 0;
	float startTime = 0;
};

