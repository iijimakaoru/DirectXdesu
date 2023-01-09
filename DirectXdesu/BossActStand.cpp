#include "BossActStand.h"
#include "Boss.h"
#include "BossBulletManager.h"

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

	BossBulletManager::GetInstance()->WaveBullet(boss->object.transform.pos, { 2,2,2 }, { 2,2,2 }, 10, 0, 20);
}

void BossActStand::Draw()
{

}
