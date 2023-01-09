#include "BossActStamp.h"
#include "Boss.h"
#include "ViewProjection.h"
#include "Player.h"
#include "BossBulletManager.h"

#include "BossActStand.h"

BossActStamp::BossActStamp()
{
	timer = 20;
	phase = 1;
	inpact = false;
}

void BossActStamp::Update()
{
	Boss* boss = Boss::nowBoss;

	Player* player = Player::nowPlayer;

	BulletAngle = PI * 2;
	Vector3 bulletScale = { 5,5,5 };
	float velocity = 0;

	if (phase == 1)
	{
		if (boss->object.transform.pos.y > 10)
		{
			boss->object.transform.pos.y -= 1;
		}

		if (boss->object.transform.pos.y <= 10)
		{
			if (timer > 0)
			{
				timer--;
			}

			if (timer == 0)
			{
				phase++;
			}
		}
	}

	if (phase == 2)
	{
		if (boss->object.transform.pos.y < 300)
		{
			boss->object.transform.pos.y += 30;
		}

		if (boss->object.transform.pos.y >= 300)
		{
			phase++;
			timer = 60;
		}
	}

	if (phase == 3)
	{
		boss->object.transform.pos.x = player->object.transform.pos.x;
		boss->object.transform.pos.z = player->object.transform.pos.z;

		if (timer > 0)
		{
			timer--;
		}

		if (timer == 0)
		{
			phase++;
			timer = 60;
		}
	}

	if (phase == 4)
	{
		if (timer > 0)
		{
			timer--;
		}

		if (timer == 0)
		{
			boss->object.transform.pos.y -= 100;

			if (boss->object.transform.pos.y <= 0)
			{
				boss->object.transform.pos.y = 0;
				if (!inpact)
				{
					while (velocity < XMConvertToRadians(360))
					{
						BossBulletManager::GetInstance()->WaveBullet(
							{ boss->object.transform.pos.x ,boss->object.transform.pos.y,boss->object.transform.pos.z },
							bulletScale, { 0,0,0 }, 5, BulletAngle + velocity, 20);
						velocity += XMConvertToRadians(5.0f);
					}
					inpact = true;
				}

				phase++;
				timer = 120;
			}
		}
	}

	if (phase == 5)
	{
		if (--timer < 0)
		{
			boss->nowBoss->actState = std::make_unique<BossActStand>();
		}
	}
}

void BossActStamp::Draw()
{
}
