#include "BossBullet.h"

void BossBullet::Init(KModel* model)
{
	// �I�u�W�F�N�g�̏�����
	object.Initialize();
	// ���f���̓ǂݍ���
	model_ = model;
	object.LoadModel(model_);
	// �e�N�X�`��
	texture.CreateTexture("Resources/texture/", "playerColor.png");
}

void BossBullet::SetInfo(const Vector3& position, const Vector3& velocity, const Vector3& scale, const Vector3& rotation, const float lifeTime)
{
	object.transform.pos = position;
	object.transform.scale = scale;

	// ��]�ʓǂݍ���
	this->rotation = rotation;
	// �����ǂݍ���
	this->velocity = velocity;
	// �傫���ǂݍ���
	this->scale = scale;

	lifeTimer = lifeTime;
}

void BossBullet::Update(ViewProjection& viewProjection)
{
	object.transform.pos += velocity;

	object.transform.rot += rotation;

	if (--lifeTimer <= 0)
	{
		isDead = true;
	}

	if (isDead)
	{
		object.transform.rot = { 0,0,0 };
	}

	object.Update(viewProjection);
}

void BossBullet::Draw()
{
	object.Draw(&texture);
}
