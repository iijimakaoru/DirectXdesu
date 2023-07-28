#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(KModel* model, KGPlin* pipeline_,
	const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed)
{
	// �C���X�^���X����
	PlayerBullet* playerBullet = new PlayerBullet();
	if (playerBullet == nullptr)
	{
		return nullptr;
	}

	// ������
	playerBullet->Init(model, pipeline_);
	playerBullet->Set(pos, vec_, rot_, bulletSpeed);

	return playerBullet;
}

void PlayerBullet::Init(KModel* model_, KGPlin* pipeline_)
{
	// ���f������
	model = model_;

	// �p�C�v���C������
	pipeline = pipeline_;

	// �I�u�W�F�N�g����
	object3d.reset(KObject3d::Create(model,pipeline));
	object3d->transform.scale = { 10.0f,10.0f,10.0f };
}
