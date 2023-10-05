#include "EnemyBullet.h"

EnemyBullet* EnemyBullet::Create(KModel* model, KGPlin* pipeline_,
	const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed)
{
	// �C���X�^���X����
	EnemyBullet* enemyBullet = new EnemyBullet();
	if (enemyBullet == nullptr)
	{
		return nullptr;
	}

	// ������
	enemyBullet->Init(model, pipeline_);
	enemyBullet->Set(pos, vec_, rot_, bulletSpeed);

	return enemyBullet;
}

void EnemyBullet::Init(KModel* model_, KGPlin* pipeline_)
{
	// ���f������
	model = model_;

	// �p�C�v���C������
	pipeline = pipeline_;

	// �I�u�W�F�N�g����
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline);
	object3d->LoadModel(model);
	object3d->transform.scale = { 3.0f,3.0f,3.0f };
}
