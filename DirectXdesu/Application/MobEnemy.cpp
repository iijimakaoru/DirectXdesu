#include "MobEnemy.h"
#include "MyMath.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#include "Player.h"

void MobEnemy::Init(KModel* model_, KGPlin* pipeline_)
{
	// ���f������
	model = model_;

	// �p�C�v���C��
	pipeline = pipeline_;

	// �I�u�W�F�N�g����
	object3d.reset(KObject3d::Create(model, pipeline));

	isDead = false;
}

void MobEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	object3d->Update(viewPro);
}

void MobEnemy::Draw()
{
	if (!isDead)
	{
		object3d->Draw();
	}
}

void MobEnemy::OnCollision()
{
	ParticleManager::GetInstance()->CallExp(GetWorldPos());
	isDead = true;
}

void MobEnemy::Attack()
{
	assert(player);

	// �N�[���^�C���o��
	coolTimer++;

	if (coolTimer >= max(coolTimer, coolTime))
	{
		// �e�̑��x
		const float kBulletSpeed = 1.0f;

		// ���L�����̃��[���h���W
		KMyMath::Vector3 pPos = player->GetWorldPos();

		// ���[���h���W
		KMyMath::Vector3 ePos = GetWorldPos();

		// �����x�N�g��
		KMyMath::Vector3 vec = pPos - ePos;

		// �e����
		BulletManager::GetInstance()->EnemyBulletShot(ePos, vec, { 1,1,1 }, kBulletSpeed);

		// �N�[���^�C��������
		coolTimer = 0;
	}
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