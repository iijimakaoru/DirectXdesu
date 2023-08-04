#include "AppearEnemy.h"
#include "Player.h"
#include "BulletManager.h"
#include "Ease.h"

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

	startScale = { 0,0,0 };

	endScale = { 4.0f,4.0f,4.0f };

	object3d->transform.scale = startScale;

	isAppear = true;

	easeTimer = 0;

	isDead = false;
}

void AppearEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	// �o�����o
	if (isAppear)
	{
		Appear();
	}

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

void AppearEnemy::Appear()
{
	easeTimer += 0.05f;

	object3d->transform.scale = MyMathUtility::OutQuadVec3(startScale, endScale, easeTimer);
	object3d->transform.rot.y = 
		MyMathUtility::OutQuadFloat(0.0f, 360.0f * 2.0f, easeTimer);

	if (easeTimer >= max(easeTimer, 1))
	{
		isAppear = false;
	}
}
