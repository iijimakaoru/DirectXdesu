#include "FlyEnemy.h"
#include "Ease.h"

FlyEnemy* FlyEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos_, const KMyMath::Vector2& endPos_, const float& speed_)
{
	// インスタンス生成
	FlyEnemy* newEnemy = new FlyEnemy();
	if (newEnemy == nullptr)
	{
		return nullptr;
	}

	// 初期化
	newEnemy->Init(model_, pipeline_);

	// 初期位置セット
	newEnemy->SetStartPos(startPos_);
	newEnemy->SetEndPos(endPos_);
	newEnemy->SetSpeed(speed_);
	newEnemy->object3d->SetPos(startPos_);

	return newEnemy;
}

void FlyEnemy::Init(KModel* model_, KGPlin* pipeline_)
{
	MobEnemy::Init(model_, pipeline_);

	object3d->transform.scale = { 4,4,4 };

	isAppear = true;

	easeTimer = 0;

	coolTime = 120;
}

void FlyEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
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

	object3d->transform.pos.z += moveSpeed;

	MobEnemy::Update(viewPro, cameraPos);
}

void FlyEnemy::Draw()
{
	MobEnemy::Draw();
}

void FlyEnemy::Attack()
{
	MobEnemy::Attack();
}

void FlyEnemy::Appear()
{
	easeTimer += 1.0f;

	object3d->transform.pos.x = MyEase::InCubicFloat(startPos.x, endPos.x, easeTimer / easeTime);
	object3d->transform.pos.y = MyEase::InCubicFloat(startPos.y, endPos.y, easeTimer / easeTime);

	if (easeTimer >= max(easeTimer, easeTime))
	{
		isAppear = false;
	}
}

void FlyEnemy::SetStartPos(const KMyMath::Vector3& startPos_)
{
	startPos = startPos_;
}

void FlyEnemy::SetEndPos(const KMyMath::Vector2& endPos_)
{
	endPos = endPos_;
}

void FlyEnemy::SetSpeed(const float& speed)
{
	moveSpeed = speed;
}
