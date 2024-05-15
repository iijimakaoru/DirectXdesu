#include "RailMain.h"

const float RailMain::advanceSpeed = 0.5f;

void RailMain::Init(const KMyMath::Vector3& startPos) { transform_.SetPos(startPos); }

void RailMain::Update() {
	// 移動速度
	KMyMath::Vector3 velocity;

	if (isAdvance) {
		velocity.z = advanceSpeed;
	}

	transform_.TransUpdate();
}
