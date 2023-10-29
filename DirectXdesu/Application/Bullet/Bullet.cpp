/**
 * @file Bullet.cpp
 * @brief 弾の基底
 * @author 飯島 薫
 */

#include "Bullet.h"

void Bullet::Set(const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed_)
{
	object3d->SetPos(pos_);
	this->vec = vec_;
	vec = MyMathUtility::MakeNormalize(vec) * bulletSpeed_;
	object3d->SetRot(rot_);

	lifeTimer = 120;

	isDead = false;
}

void Bullet::Update(ViewProjection* viewPro_)
{
	if (!isDead)
	{
		lifeTimer--;
		object3d->AddSetPos(vec);

		if (lifeTimer <= 0)
		{
			isDead = true;
		}
	}

	object3d->Update(viewPro_);
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
	result.x = object3d->GetMatWorld().m[3][0];
	result.y = object3d->GetMatWorld().m[3][1];
	result.z = object3d->GetMatWorld().m[3][2];

	return result;
}
