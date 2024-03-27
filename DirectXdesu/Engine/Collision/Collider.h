#pragma once
#include "MyMath.h"

class Collider {
public:
	// 当たった時の処理
	virtual void OnCollision(){};

	// ワールドポジション
	virtual KMyMath::Vector3 GetWorldPosition() = 0;

	// 半径セット
	void SetRadius(const float& radius);

	// 半径取得
	const float& GetRadius() const;

private:
	// 半径
	float radius_ = 1.0f;
};
