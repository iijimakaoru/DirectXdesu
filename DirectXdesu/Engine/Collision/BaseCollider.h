#pragma once
#include "CollisionTypes.h"
#include <stdint.h>
#include "CollisionInfo.h"
#include "KObject3d.h"

class BaseCollider {
public:
	BaseCollider() = default;
	// 仮想デストラクタ
	virtual ~BaseCollider() = default;

	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType();

	inline void OnCollision(const CollisionInfo& info);

	inline void SetObject3d(KObject3d* object3d);

	inline KObject3d* GetObject3d();

protected:
	KObject3d* object3d_ = nullptr;

	CollisionShapeType shapeType = CollisionShapeType::Shape_Unknown;
};
