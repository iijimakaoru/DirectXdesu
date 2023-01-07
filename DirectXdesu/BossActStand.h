#pragma once
#include "BossActState.h"

class BossActStand : public BossActState
{
private:
	float angleY = 0;

	const float maxTimer = 2.5f;
	float timer = 0;

	bool isInit = false;

	int actNum = 0;
public:
	BossActStand();

	void Update() override;
	void Draw() override;
};

