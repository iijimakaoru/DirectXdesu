#include "AppearEnemy.h"
#include "Player.h"
#include "BulletManager.h"

AppearEnemy* AppearEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos)
{
	// �C���X�^���X����
	AppearEnemy* appearEnemy = new AppearEnemy();
	if (appearEnemy == nullptr)
	{
		return nullptr;
	}

	// ������
	appearEnemy->Init(model_,pipeline_);

	// �����ʒu�Z�b�g
	appearEnemy->object3d->SetPos(pos);

	return appearEnemy;
}

void AppearEnemy::Init(KModel* model_, KGPlin* pipeline_)
{
	// ���f������
	model = model_;

	// �p�C�v���C��
	pipeline = pipeline_;

	// �I�u�W�F�N�g����
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->transform.scale = { 4.0f,4.0f,4.0f };
}

void AppearEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	if (!isDead)
	{
		if (KInput::GetInstance()->IsTrigger(DIK_2))
		{
			Attack();
		}

		if (object3d->transform.pos.z <= min(object3d->transform.pos.z, cameraPos.z))
		{
			isDead = true;
		}
	}

	object3d->Update(viewPro);
}

void AppearEnemy::Attack()
{
	assert(player);

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
}
