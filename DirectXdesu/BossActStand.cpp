#include "BossActStand.h"
#include "Boss.h"
#include "BossBulletManager.h"

BossActStand::BossActStand()
{
	
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

	BossBulletManager::GetInstance()->WaveBullet(boss->object.transform.pos, { 2,2,2 }, { 2,2,2 }, 10, 0, 20);
}

void BossActStand::Draw()
{

}
