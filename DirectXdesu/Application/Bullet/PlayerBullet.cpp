/**
 * @file PlayerBullet.cpp
 * @brief 自機の弾
 * @author 飯島 薫
 */

#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(
    KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_,
    const KMyMath::Vector3& rot_, const float bulletSpeed_, const float BulletPower_) {
	// インスタンス生成
	PlayerBullet* playerBullet = new PlayerBullet();
	if (playerBullet == nullptr) {
		return nullptr;
	}

	// 初期化
	playerBullet->Init(model_, pipeline_);
	playerBullet->Set(pos_, vec_, rot_, bulletSpeed_, BulletPower_);

	return playerBullet;
}

void PlayerBullet::Init(KModel* model_, KGPlin* pipeline_) {
	// モデル生成
	model = model_;

	// パイプライン生成
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->SetScale({5.0f, 5.0f, 5.0f});
}
