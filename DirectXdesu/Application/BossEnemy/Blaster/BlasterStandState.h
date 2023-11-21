#pragma once
#include "ActState.h"
#include "MyMath.h"

class BlasterStandState : public ActState
{
public:
	// コンストラクタ
	BlasterStandState();

	// 更新
	void Update() override;

private:
	KMyMath::Vector3 unitPos = { 0.0f,0.0f,0.0f };
};

