#include "BaseCollider.h"

inline CollisionShapeType BaseCollider::GetShapeType() { return shapeType; }

inline void BaseCollider::OnCollision(const CollisionInfo& info) { CollisionInfo hoge = info; }

inline void BaseCollider::SetObject3d(KObject3d* object3d) { object3d_ = object3d; }

inline KObject3d* BaseCollider::GetObject3d() { return object3d_; }
