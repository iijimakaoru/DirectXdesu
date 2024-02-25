#pragma once
#include "ActState.h"

class BlasterUnitLazer : public ActState {
public:
	BlasterUnitLazer();

	// 更新
	void Update() override;

	enum Phase { CubeClose, CubeOpen, GoCube, End };

	enum CubeForm { Alpha, Beta, Gamma };

private:
	/// <summary>
	/// ユニット消し
	/// </summary>
	void CubeCloseAct();
	/// <summary>
	/// ユニット出現
	/// </summary>
	void CubeOpenAct();

	void CubeSet();
	void GoCubeAct();
	void EndAct();

private:
	Phase actsPhase;

	CubeForm cubeForm;

	float startTimer = 0;
	float startTime = 0;
};
