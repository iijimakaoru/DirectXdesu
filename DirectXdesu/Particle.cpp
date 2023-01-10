#include "Particle.h"
#include "Ease.h"

void Particle::Init(KModel* model)
{
	// �I�u�W�F�N�g�̏�����
	object.Initialize();
	// ���f���̓ǂݍ���
	model_ = model;
	object.LoadModel(model_);
	// �e�N�X�`��
	texture.CreateTexture("Resources/texture/", "damageColor.png");
}

void Particle::SetInfo(const Vector3& pos, const Vector3& velocity, const Vector3& scale, const Vector3& rot, const float kLifeTime)
{
	object.transform.pos = pos;
	object.transform.scale = { 0,0,0 };

	// ��]�ʓǂݍ���
	this->rotation = rot;
	// �����ǂݍ���
	this->velocity = velocity;
	// �傫���ǂݍ���
	this->scale = scale;

	maxLifeTimer = kLifeTime;
	lifeTimer = maxLifeTimer;
}

void Particle::Update(ViewProjection& view)
{
	if (isExp)
	{
		object.transform.scale.x = EaseIn(object.transform.scale.x, scale.x, 1.0f / lifeTimer, 0.5f);
		object.transform.scale.y = EaseIn(object.transform.scale.y, scale.y, 1.0f / lifeTimer, 0.5f);
		object.transform.scale.z = EaseIn(object.transform.scale.z, scale.z, 1.0f / lifeTimer, 0.5f);
	}
	else
	{
		object.transform.scale.x = EaseIn(scale.x, 0, 1.0f / lifeTimer, 0.5f);
		object.transform.scale.y = EaseIn(scale.y, 0, 1.0f / lifeTimer, 0.5f);
		object.transform.scale.z = EaseIn(scale.z, 0, 1.0f / lifeTimer, 0.5f);
	}

	if (lifeTimer >= maxLifeTimer / 2)
	{
		isExp = true;
	}
	else
	{
		isExp = false;
	}

	// �ړ�
	object.transform.pos += velocity;

	// ��]
	object.transform.rot += rotation;

	// �A�v�f
	object.Update(view);

	// �����鏈��
	if (--lifeTimer <= 0)
	{
		isDead = true;
	}
}

void Particle::Draw()
{
	object.Draw(&texture);
}
