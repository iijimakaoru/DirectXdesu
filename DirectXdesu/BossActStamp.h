#pragma once
#include "BossActState.h"

class BossActStamp : public BossActState
{
public:
	BossActStamp();

	void Update() override;
	void Draw() override;

private:
	float timer;

	int phase = 0;

	const float fallSpeed = 0.5f;

	float BulletAngle = 0;

	int shotCount = 0;

	bool inpact = false;
};

