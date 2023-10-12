#pragma once
#include "Bullet.h"

/**
 * @file EnemyBullet.h
 * @brief 敵の弾
 * @author 飯島 薫
 */

class EnemyBullet : public Bullet
{
public:
	static EnemyBullet* Create(KModel* model, KGPlin* pipeline_,
		const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed);

public:
	// 初期化
	void Init(KModel* model_, KGPlin* pipeline_) override;
};

