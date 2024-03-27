#include "SphereCollider.h"

void SphereCollider::Update() {
	Sphere::center_ = offset_;
	Sphere::radius_ = radius_;
}

inline void SphereCollider::SetRadius(float radius) { radius_ = radius; }
