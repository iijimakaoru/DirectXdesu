#pragma once
#include "Movie.h"

class StageStart : public BaseMovie {
public:
	enum CameraPhase { LookDown = 0, TopRight = 1, Back = 2, Center = 3, Come = 4, Exit = 5 };

	StageStart();

	void Update() override;

private:
	void LookDownPhase();
	void TopRightPhase();
	void BackPhase();
	void CenterPhase();
	void ComePhase();
	void ExitPhase();

private:
	CameraPhase cameraPhase = LookDown;
};
