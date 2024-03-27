#pragma once

class BaseCollider;

struct CollisionInfo {
public:
	CollisionInfo(BaseCollider* baseCollider) { baseCollider_ = baseCollider; }

private:
	BaseCollider* baseCollider_;
};