#pragma once
#include "BossActState.h"

class BossActAimShot : public BossActState
{
public:
	BossActAimShot();

	void Update() override;
	void Draw() override;

private:
	float timer = 0;

	float coolTimer = 0;

	int phase = 0;
};

