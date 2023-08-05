#include "MobEnemy.h"
#include "MyMath.h"
#include "ParticleManager.h"

void MobEnemy::Draw()
{
	if (!isDead)
	{
		object3d->Draw();
	}
}

void MobEnemy::OnCollision()
{
	isDead = true;
}

const KMyMath::Vector3 MobEnemy::GetWorldPos() const
{
	// ���[���h���W�i�[�ϐ�
	KMyMath::Vector3 result;

	// ���[���h�s��̕��s�ړ������擾
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}