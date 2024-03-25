#pragma once
#include "AudioManager.h"
#include "Movie.h"
#include "SceneManager.h"
#include "SceneChange.h"
#include "BulletManager.h"

class StageClear : public BaseMovie {
public:
	enum CameraPhase {
		Insert = 0,
		Round = 1,
		Fly = 2,
		BlackOut = 3,
		GoResult = 4
	};

	StageClear();

	void Update() override;

private:
	void InsertPhase();

	void RoundPhase();

	void FlyPhase();

	void BlackOutPhase();

	void GoResultPhase();

private:
	AudioManager* audioManager_ = nullptr;

	SceneChange* sceneChange_ = nullptr;

	SceneManager* sceneManager_ = nullptr;

	BulletManager* bulletManager_ = nullptr;

	// 制御点
	KMyMath::Vector3 start;
	KMyMath::Vector3 p1;
	KMyMath::Vector3 p2;
	KMyMath::Vector3 end;

	CameraPhase cameraPhase = CameraPhase::Insert;
};
