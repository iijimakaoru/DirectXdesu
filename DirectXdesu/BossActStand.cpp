#include "BossActStand.h"
#include "BossActAimShot.h"
#include "BossActStamp.h"
#include "Boss.h"
#include "BossBulletManager.h"
#include "MyMath.h"

BossActStand::BossActStand()
{
	timer = 60;
}

void BossActStand::Update()
{
	Boss* boss = Boss::nowBoss;

	if (boss->object.transform.pos.y < 20)
	{
		boss->object.transform.pos.y += 0.5f;
	}
	else if (boss->object.transform.pos.y > 20)
	{
		boss->object.transform.pos.y -= 0.5f;
	}
	else
	{
		boss->object.transform.pos.y = 20;
	}

	boss->object.transform.rot.y += 0.1f;

	if (--timer < 0)
	{
		int mode = MyMath::GetInstance()->GetRand(0, 2);

		if (mode == 1)
		{
			boss->actState = std::make_unique<BossActAimShot>();
		}
		else
		{
			boss->actState = std::make_unique<BossActStamp>();
		}
	}
}

void BossActStand::Draw()
{

}
