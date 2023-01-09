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

	boss->object.transform.pos.y -= 5;

	if (boss->object.transform.pos.y <= 0)
	{
		boss->object.transform.pos.y = 0;
	}

	if (boss->object.transform.pos.y == 0)
	{
		if (--timer < 0)
		{
			boss->actState = std::make_unique<BossActStand>();
		}
	}
}

void BossActGap::Draw()
{
}
