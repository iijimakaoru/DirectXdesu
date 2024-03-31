/**
 * @file EnemyBullet.cpp
 * @brief 敵の弾
 * @author 飯島 薫
 */

#include "EnemyBullet.h"

EnemyBullet* EnemyBullet::Create(
    KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_,
    const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed_,
    const float BulletPower_) {
	// インスタンス生成
	EnemyBullet* enemyBullet = new EnemyBullet();
	if (enemyBullet == nullptr) {
		return nullptr;
	}

	// 初期化
	enemyBullet->Init(model_, pipeline_);
	enemyBullet->Set(pos_, vec_, rot_, bulletSpeed_, BulletPower_);

	return enemyBullet;
}

void EnemyBullet::Init(KModel* model_, KGPlin* pipeline_) {
	SetCollisionAttribute(Collider::Attribute::Enemys);
	SetCollisionMask((uint32_t)~Collider::Attribute::Enemys);

	// モデル生成
	model = model_;

	// パイプライン生成
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->GetTransform().SetScale({1.0f, 1.0f, 1.0f});
}
