#include "BossEnemy.h"
#include "ParticleManager.h"

void BossEnemy::Draw()
{
	object3d->Draw();
}

const KMyMath::Vector3 BossEnemy::GetWorldPos() const
{
	// ���[���h���W�i�[�ϐ�
	KMyMath::Vector3 result;

	// ���[���h�s��̕��s�ړ������擾
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
