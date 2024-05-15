#pragma once
#include "KObject3d.h"

class RailMain {
public:
	void Init(const KMyMath::Vector3& startPos);

	void Update();

public:
	// 前進スピード
	static const float advanceSpeed;

private:
	Transform transform_;

	// 前進するか
	bool isAdvance = true;
};
