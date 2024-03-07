#pragma once
#include "Bullet.h"

/**
 * @file PlayerBullet.h
 * @brief プレイヤーの弾
 * @author 飯島 薫
 */

class PlayerBullet : public Bullet {
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pipeline_">パイプライン</param>
	/// <param name="pos">初期位置</param>
	/// <param name="vec_">動く向き</param>
	/// <param name="rot_">回転角</param>
	/// <param name="bulletSpeed">弾速</param>
	/// <returns></returns>
	static PlayerBullet* Create(
	    KModel* model, KGPlin* pipeline_, const KMyMath::Vector3& pos,
	    const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed,
	    const float BulletPower_);

public:
	// 初期化
	void Init(KModel* model_, KGPlin* pipeline_) override;
};