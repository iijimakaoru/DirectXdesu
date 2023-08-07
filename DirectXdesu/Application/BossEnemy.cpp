#include "BossEnemy.h"
#include "ParticleManager.h"

void BossEnemy::Draw()
{
	object3d->Draw();
}

const KMyMath::Vector3 BossEnemy::GetWorldPos() const
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}

void BossEnemy::OnCollision()
{
	ParticleManager::GetInstance()->CallExp(GetWorldPos());
	HP--;
}
