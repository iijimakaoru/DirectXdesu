#include "BossActStand.h"
#include "Boss.h"

BossActStand::BossActStand()
{
	
}

void BossActStand::Update()
{
	Boss* boss = Boss::nowBoss;

	Vector3 pos = boss->object.transform.pos;

	Vector3 diffPos = pos - boss->object.transform.pos;
	if (diffPos.Length() <= 0.001f)
	{
		boss->object.transform.pos = pos;
	}
	else
	{
		boss->object.transform.pos += diffPos / 10.0f;
	}

	boss->object.transform.rot.y += 0.1f;
}

void BossActStand::Draw()
{

}
