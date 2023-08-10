#include "FlyEnemy.h"

FlyEnemy* FlyEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos, const KMyMath::Vector2& endPos)
{
	// �C���X�^���X����
	FlyEnemy* newEnemy = new FlyEnemy();
	if (newEnemy == nullptr)
	{
		return nullptr;
	}

	// ������
	newEnemy->Init(model_, pipeline_);

	// �����ʒu�Z�b�g
	newEnemy->object3d->SetPos(startPos);

	return newEnemy;
}

void FlyEnemy::Init(KModel* model_, KGPlin* pipeline_)
{
	MobEnemy::Init(model_, pipeline_);
}

void FlyEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
}

void FlyEnemy::Attack()
{
}

void FlyEnemy::Appear()
{
}
