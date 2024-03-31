#pragma once
#include "MyMath.h"

class Collider {
public:
	enum Attribute {
		Players       = 0b1,
		Enemys        = 0b1 << 1,
	};

	// 当たった時の処理
	virtual void OnCollision(){};

	// ワールドポジション
	virtual KMyMath::Vector3 GetWorldPosition() = 0;

	// 半径セット
	void SetRadius(const float& radius);

	// 半径取得
	const float& GetRadius() const;

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
};
