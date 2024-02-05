#pragma once
#include "ActState.h"

class BlasterUnitLazer : public ActState {
public:
	BlasterUnitLazer();

	// 更新
	void Update() override;

	enum Phase { CubeClose, CubeOpen, GoCube, End };

private:
	void CubeCloseAct();
	void CubeOpenAct();
	void GoCubeAct();
	void EndAct();

private:
	Phase actsPhase;

	float startTimer = 0;
	float startTime = 0;
};
