#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector3.h"
#include "KTexture.h"

const int BulletNum = 300;

class BossBullet
{
public:
	void Init(KModel* model);
	void SetInfo(const Vector3& position,
		const Vector3& velocity,
		const Vector3& scale,
		const Vector3& rotation,
		const float lifeTime);
	void Update(ViewProjection& viewProjection);
	void Draw();
	bool IsDead()const { return isDead; }
	void Revive() { isDead = false; }

	KObject3d object;

	KModel* model_;

	Vector3 velocity;

	Vector3 rotation;

	Vector3 scale;

	bool isDead = true;

	float lifeTimer = 0;

	KTexture texture;
};

