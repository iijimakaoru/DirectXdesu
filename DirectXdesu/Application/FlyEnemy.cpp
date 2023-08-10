#include "FlyEnemy.h"

FlyEnemy* FlyEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos, const KMyMath::Vector2& endPos)
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
