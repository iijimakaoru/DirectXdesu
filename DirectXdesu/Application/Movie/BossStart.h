#pragma once
#include "Movie.h"
#include "BulletManager.h"
#include "AudioManager.h"
#include "SceneChange.h"

class BossStart : public BaseMovie {
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
	 
	void DescentPhase();

	void ApproachPhase();

	void OpenUnitPhase();

	void DistantPhase();
	 
	void BlackOutPhase();

	void SetBossPhase();

	void ExitPhase();

	void BreakTimePhase1();

	void BreakTimePhase2();

private:
	CameraPhase cameraPhase = WaitBlackOut;
	BulletManager* bulletManager_ = nullptr;
	AudioManager* audioManager_ = nullptr;
	SceneChange* sceneChange_ = nullptr;
};
