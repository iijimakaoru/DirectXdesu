#pragma once
#include "ActState.h"
class BlasterAimState : public ActState {
public:
	// コンストラクタ
	BlasterAimState();
	// 更新
	void Update() override;

	enum Phase { STAND, CHARGE, SHOT, END };

private:
	void BulletShot();

	void StandAct();

	void ChargeAct();

	void ShotAct();

	void EndAct();

private:
	float angle = 0;
	float startTimer = 0;
	float startTime = 0;

	float angleSpeed = 0;

	Phase actsPhase;
};
