#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include <DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere {
public:
	SphereCollider(KMyMath::Vector3 offset = {0, 0, 0}, float radius = 1.0f)
		: offset_(offset), radius_(radius) {
		shapeType = CollisionShapeType::CollisionShape_Sphere;
	}

	void Update() override;

	inline void SetRadius(float radius);

private:
	KMyMath::Vector3 offset_;
	float radius_;
};
