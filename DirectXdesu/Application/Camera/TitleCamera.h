#pragma once
#include "Camera.h"

class TitleCamera : public Camera
{
public:
	TitleCamera()
	{
		
	}

	void Init();

	void Update();

	void RoundCamera();

	void StartRound();

private:
	KMyMath::Vector3 eye;
	KMyMath::Vector3 target;
	KMyMath::Vector3 up;

	float rotAngle = 0;

	bool isRound = false;
};

