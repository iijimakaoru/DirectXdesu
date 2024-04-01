#include "CollisionManager.h"
#include "Collision.h"

void CollisionManager::CheckAllCollisions() {
#pragma region 総当たり判定
	std::list<Collider*>::iterator itrA = colliders_.begin();

	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colB = *itrB;

			CheckCollisionPair(colA, colB);
		}
	}
#pragma endregion
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 衝突フィルタリング
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
	    !(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())) {
		return;
	}

	// ワールドポジション
	KMyMath::Vector3 colAPos = colliderA->GetWorldPosition();
	KMyMath::Vector3 colBPos = colliderB->GetWorldPosition();

	// 半径
	float colARadius = colliderA->GetRadius();
	float colBRadius = colliderB->GetRadius();

	if (MyCollisions::CheckSphereToSphere(colAPos, colBPos, colARadius, colBRadius)) {
		if (!colliderA->GetIsHit() && !colliderB->GetIsHit()) {
			colliderA->OnCollision(colliderB);

			colliderB->OnCollision(colliderA);
		}
	}
}

void CollisionManager::AddCollider(Collider* collider) { colliders_.push_back(collider); }

void CollisionManager::Reset() { colliders_.clear(); }
