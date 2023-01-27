#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector3.h"
#include "KTexture.h"
#include "ViewProjection.h"
#include "KGPlin.h"

class Particle
{
public:
	void Init(KModel* model, KGPlin* pipeline);
	void SetInfo(const Vector3& pos,
		const Vector3& velocity,
		const Vector3& scale,
		const Vector3& rot,
		const float kLifeTime);
	void Update(ViewProjection& view);
	bool IsDead() const { return isDead; }
	void Draw();
	void Revive() { isDead = false; }

	KObject3d object;

	KModel* model_;

	Vector3 velocity;

	Vector3 rotation;

	Vector3 scale;

	bool isDead = true;

	float lifeTimer = 0;

	float maxLifeTimer = 0;

	KTexture texture;

	bool isExp = false;
};

class Particle2
{
public:
	void Init(KModel* model, KGPlin* pipeline);
	void SetInfo(const Vector3& pos,
		const Vector3& velocity,
		const Vector3& scale,
		const Vector3& rot,
		const float kLifeTime);
	void Update(ViewProjection& view);
	bool IsDead() const { return isDead; }
	void Draw();
	void Revive() { isDead = false; }

	KObject3d object;

	KModel* model_;

	Vector3 velocity;

	Vector3 rotation;

	Vector3 scale;

	bool isDead = true;

	float lifeTimer = 0;

	float maxLifeTimer = 0;

	KTexture texture;

	bool isExp = false;
};