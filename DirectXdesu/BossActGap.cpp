#include "BossActGap.h"
#include "BossActStand.h"
#include "Boss.h"

BossActGap::BossActGap()
{
	timer = 120;
}

void BossActGap::Update()
{
	Boss* boss = Boss::nowBoss;

	if (--timer < 0)
	{
		boss->actState = std::make_unique<BossActStand>();
	}
}

void BossActGap::Draw()
{
}
