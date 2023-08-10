#include "AppearEnemy.h"
#include "BulletManager.h"
#include "Ease.h"

AppearEnemy* AppearEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos)
{
	// インスタンス生成
	AppearEnemy* newEnemy = new AppearEnemy();
	if (newEnemy == nullptr)
	{
		return nullptr;
	}

	// 初期化
	newEnemy->Init(model_, pipeline_);

	// 初期位置セット
	newEnemy->object3d->SetPos(pos);

	return newEnemy;
}

void AppearEnemy::Init(KModel* model_, KGPlin* pipeline_)
{
	MobEnemy::Init(model_, pipeline_);

	startScale = { 0,0,0 };

	endScale = { 4.0f,4.0f,4.0f };

	object3d->transform.scale = startScale;

	isAppear = true;

	easeTimer = 0;
}

void AppearEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	// 出現演出
	if (isAppear)
	{
		Appear();
	}

	MobEnemy::Update(viewPro, cameraPos);
}

void AppearEnemy::Attack()
{
	MobEnemy::Attack();
}

void AppearEnemy::Appear()
{
	easeTimer += 0.05f;

	object3d->transform.scale = MyEase::OutQuadVec3(startScale, endScale, easeTimer);
	object3d->transform.rot.y = MyEase::OutQuadFloat(0.0f, 360.0f * 2.0f, easeTimer);

	if (easeTimer >= max(easeTimer, 1))
	{
		isAppear = false;
	}
}
