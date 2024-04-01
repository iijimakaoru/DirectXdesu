#include "Collider.h"

void Collider::SetRadius(const float& radius) { radius_ = radius; }

const float& Collider::GetRadius() const { return radius_; }

const bool& Collider::GetIsHit() const { return isHit_; }

void Collider::SetIsHit(bool isHit) { isHit_ = isHit; }

void Collider::SetCollisionAttribute(uint32_t collisionAttribute) {
	collisionAttribute_ = collisionAttribute;
}

const uint32_t& Collider::GetCollisionAttribute() const { return collisionAttribute_; }

void Collider::SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

const uint32_t& Collider::GetCollisionMask() const { return collisionMask_; }
