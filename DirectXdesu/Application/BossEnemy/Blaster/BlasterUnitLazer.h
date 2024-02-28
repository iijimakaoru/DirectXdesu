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

	void BulletShot();
	void LazerTrack();

private:
	Phase actsPhase;

	CubeForm cubeForm;

	float startTimer = 0;
	float startTime = 0;

	bool isStay = false;
	uint32_t stayTimer = 0;
	uint32_t stayTime = 30;

	uint32_t unitCount = 0;

	uint32_t coolTimer = 0;
};
