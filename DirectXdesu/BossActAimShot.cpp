#include "BossActAimShot.h"
#include "Boss.h"
#include "Player.h"
#include "BossBulletManager.h"
#include "BossActGap.h"

BossActAimShot::BossActAimShot()
{
	phase = 1;
}

void BossActAimShot::Update()
{
	Boss* boss = Boss::nowBoss;

	Player* player = Player::nowPlayer;

	if (phase == 1)
	{
		if (boss->object.transform.pos.y < 90)
		{
			boss->object.transform.pos.y += 10;
		}

		if (boss->object.transform.pos.y >= 90)
		{
			phase++;
			timer = 30;
		}
	}

	if (phase == 2)
	{
		if (--timer < 0)
		{
			phase++;
			timer = 600;
		}
	}

	if (phase == 3)
	{
		timer--;

		Vector3 velocity = player->object.transform.pos - boss->object.transform.pos;

		velocity.Normalize();

		if (--coolTimer < 0)
		{
			BossBulletManager::GetInstance()->AimBullet(boss->object.transform.pos, { 5,5,5 }, { 1,1,1 }, velocity, 4, 100);
			coolTimer = 10;
		}

		if (timer <= 0)
		{
			boss->actState = std::make_unique<BossActGap>();
		}
	}
}

void BossActAimShot::Draw()
{
}
