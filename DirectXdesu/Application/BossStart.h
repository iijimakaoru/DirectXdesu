#pragma once
#include "Movie.h"
#include "BulletManager.h"

class BossStart : public Movie {
public:
	enum CameraPhase {
		WaitBlackOut = 0,
		Descent = 1,
		Approach = 2,
		BreakTime1 = 3,
		OpenUnit = 4,
		BreakTime2 = 5,
		Distant = 6,
		BlackOut = 7,
		SetBoss = 8,
		Exit = 9,
	};

	BossStart();

	void Update() override;

private:
	void WaitBlackOutPhase();

private:
	CameraPhase cameraPhase = WaitBlackOut;
	BulletManager* bulletManager_ = nullptr;
};
