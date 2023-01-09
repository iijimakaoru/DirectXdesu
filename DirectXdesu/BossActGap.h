#pragma once
#include "BossActState.h"

class BossActGap : public BossActState
{
public:
	BossActGap();

	void Update() override;
	void Draw() override;

private:
	float timer = 0;
};

