#include "CanonEnemy.h"
#include "Ease.h"

CanonEnemy* CanonEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos)
{
	CanonEnemy* newEnemy = new CanonEnemy();
	if (newEnemy == nullptr)
	{
		return nullptr;
	}

	newEnemy->Init(model_, pipeline_);

	newEnemy->SetStartPos(pos);

	newEnemy->object3d->SetPos(pos);

	return newEnemy;
}

void CanonEnemy::Init(KModel* model_, KGPlin* pipeline_)
{
	MobEnemy::Init(model_, pipeline_);

	isAppear = true;

	easeTimer = 0;

	object3d->transform.scale = { 6.0f,6.0f,6.0f };

	coolTime = 30;
}

void CanonEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	// 出現演出
	if (isAppear)
	{
		Appear();
	}
	else
	{
		if (!isDead)
		{
			Attack();

			if (object3d->transform.pos.z <= min(object3d->transform.pos.z, cameraPos.z))
			{
				isDead = true;
			}
		}
	}

	MobEnemy::Update(viewPro, cameraPos);
}

void CanonEnemy::Draw()
{
	MobEnemy::Draw();
}

void CanonEnemy::Attack()
{
	MobEnemy::Attack();
}

void CanonEnemy::Appear()
{
	easeTimer += 1.0f;

	object3d->transform.pos.y = MyEase::InOutCubicFloat(startPos.y, startPos.y + 20, easeTimer / easeTime);
	object3d->transform.rot.y = MyEase::OutCubicFloat(0, 360 * 2, easeTimer / easeTime);

	if (easeTimer >= max(easeTimer, easeTime))
	{
		isAppear = false;
	}
}

void CanonEnemy::SetStartPos(const KMyMath::Vector3& startPos_)
{
	startPos = startPos_;
}