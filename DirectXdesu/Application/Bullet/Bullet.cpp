/**
 * @file Bullet.cpp
 * @brief 弾の基底
 * @author 飯島 薫
 */

#include "Bullet.h"

void Bullet::Set(const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed)
{
	object3d->transform.pos = pos;
	this->vec = vec_;
	vec = MyMathUtility::MakeNormalize(vec) * bulletSpeed;
	object3d->transform.rot = rot_;

	lifeTimer = 120;

	isDead = false;
}

void Bullet::Update(ViewProjection* viewPro)
{
	if (!isDead)
	{
		lifeTimer--;
		object3d->transform.pos += vec;

		if (lifeTimer <= 0)
		{
			isDead = true;
		}
	}

	object3d->Update(viewPro);
}

void Bullet::Draw()
{
	if (!isDead)
	{
		object3d->Draw();
	}
}

void Bullet::OnCollision()
{
	isDead = true;
}

KMyMath::Vector3 Bullet::GetWorldPos()
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}
