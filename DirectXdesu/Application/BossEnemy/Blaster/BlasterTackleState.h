#pragma once
#include "ActState.h"
#include "MyMath.h"

class BlasterTackleState : public ActState {
public:
	BlasterTackleState();

	void Update() override;

	private:
	void TackleVec();
	void Tackle();

private:
	float angle = 0;
	float startTimer = 0;
	float startTime = 0;

	KMyMath::Vector3 tackleVec;
};
