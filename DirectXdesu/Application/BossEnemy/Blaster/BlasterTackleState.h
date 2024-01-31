#pragma once
#include "ActState.h"
#include "MyMath.h"

class BlasterTackleState : public ActState {
public:
	BlasterTackleState();

	void Update() override;

	enum Phase {
		CubeClose,
		CubeOpen,
		Aim, StandTackle,
		GoTackle,
		BackPos,
		End
	};

private:
	void TackleVec();
	void Tackle();

	void CubeCloseAct();
	void CubeOpenAct();
	void AimAct();
	void StandTackleAct();
	void GoTackleAct();
	void BackPosAct();
	void EndAct();

private:
	float angle = 0;
	float startTimer = 0;
	float startTime = 0;

	Phase actsPhase;

	KMyMath::Vector3 tackleVec;
};
