#pragma once
#include "MyMath.h"

class Collider {
public:
	enum Attribute {
		Players       = 0x00000001,
		PlayersBullet = 0x00001001,
		PlayersBom    = 0x00010001,
		Enemys        = 0x00000010,
		EnemysBullet  = 0x00001010,
		Bosss         = 0x00000100,
		BosssBullet   = 0x00001100,
		BosssLazer    = 0x00010100,
	};

	// 当たった時の処理
	virtual void OnCollision(Collider* collider) = 0;

	// ワールドポジション
	virtual KMyMath::Vector3 GetWorldPosition() = 0;

	// 半径セット
	void SetRadius(const float& radius);

	// 半径取得
	const float& GetRadius() const;

	// 当たったか
	const bool& GetIsHit() const;
	void SetIsHit(bool isHit);

	// 衝突属性(自分)付与
	void SetCollisionAttribute(uint32_t collisionAttribute);

	// 衝突属性(自分)取得
	const uint32_t& GetCollisionAttribute() const;

	// 衝突属性(相手)付与
	void SetCollisionMask(uint32_t collisionMask);

	// 衝突属性(相手)取得
	const uint32_t& GetCollisionMask() const;

private:
	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突属性(相手)
	uint32_t collisionMask_ = 0xffffffff;

	// 半径
	float radius_ = 1.0f;

	bool isHit_ = false;
};
