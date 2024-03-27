#pragma once
#include <list>
#include "Collider.h"

class CollisionManager {
public:
	void CheckAllCollisions();

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	void AddCollider(Collider* collider);

	void Reset();

private:
	std::list<Collider*> colliders_;
};
