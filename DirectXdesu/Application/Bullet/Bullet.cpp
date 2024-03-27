/**
 * @file Bullet.cpp
 * @brief 弾の基底
 * @author 飯島 薫
 */

#include "Bullet.h"

void Bullet::Set(
    const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
    const float bulletSpeed_, const float bulletPower_) {
	object3d->GetTransform().SetPos(pos_);
	this->vec = vec_;
	vec = MyMathUtility::MakeNormalize(vec) * bulletSpeed_;
	object3d->GetTransform().SetRot(rot_);

	lifeTimer = 120;

	bulletPower = bulletPower_;

	isDead = false;
}

void Bullet::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	if (!isDead) {
		lifeTimer--;
		object3d->GetTransform().AddSetPos(vec);

		if (lifeTimer <= 0) {
			isDead = true;
		}
	}

	object3d->Update(viewPro, cameraPos);
}

void Bullet::Draw() {
	if (!isDead) {
		object3d->Draw();
	}
}

void Bullet::OnCollision() { isDead = true; }

KMyMath::Vector3 Bullet::GetWorldPosition() { 
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = object3d->GetTransform().GetWorldPos();

	return result;
}

const float& Bullet::GetBulletPower() const { return bulletPower; }

const KMyMath::Vector3 Bullet::GetWorldPos() const {
	KMyMath::Vector3 result;

	result = object3d->GetTransform().GetWorldPos();

	return result;
}
