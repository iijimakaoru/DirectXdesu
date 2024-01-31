#include "BlasterStandState.h"
#include "Blaster.h"
#include "Ease.h"

BlasterStandState::BlasterStandState()
{
	isFinish = false;
	actsPhase = CubeClose;
}

void BlasterStandState::Update()
{
	switch (actsPhase) {
	case BlasterStandState::CubeClose:
		CubeCloseAct();
		break;
	case BlasterStandState::Stand:
		StandAct();
		break;
	case BlasterStandState::End:
		EndAct();
		break;
	default:
		break;
	}
}

void BlasterStandState::CubeCloseAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 15.0f;

	if (actTimer < actTime) {
		actTimer++;
		for (size_t i = 0; i < 8; i++) {
			blaster->SetUnitsScale(
			    MyEase::InCubicVec3({1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, actTimer / actTime), i);
		}
	} else {
		actTimer = 0;
		actsPhase = Stand;
	}
}

void BlasterStandState::StandAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 120.0f;
	startTime = 15.0f;
	if (actTimer == 0) {
		float posY = 6.0f;
		for (size_t i = 0; i < 8; i++) {
			if (i > 3) {
				posY = -6.0f;
			}
			blaster->SetUnitsPos({blaster->GetUnitsPos(i).x, posY, blaster->GetUnitsPos(i).z}, i);
		}
	}

	if (startTimer < startTime) {
		startTimer++;

		for (size_t i = 0; i < 8; i++) {
			blaster->SetUnitsScale(
			    MyEase::OutCubicVec3(
			        {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, startTimer / startTime),
			    i);
		}
	}

	if (actTimer < actTime) {
		if (actTimer == 0) {
			angle = 45.0f;
			float posY = 6.0f;
			for (size_t i = 0; i < 8; i++) {
				if (i > 3) {
					posY = -6.0f;
				}
				blaster->SetUnitsPos(
				    {blaster->GetUnitsPos(i).x, posY, blaster->GetUnitsPos(i).z}, i);
			}
		}

		actTimer++;

		angle += 2.0f;

		// 360を超えたら
		if (angle >= 360) {
			angle = 0;
		}

		const float radian = DirectX::XMConvertToRadians(angle);
		const float distance = 12.0f;

		for (size_t i = 0; i < 8; i++) {
			blaster->SetUnitsPos(
			    {distance * sinf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     blaster->GetUnitsPos(i).y,
			     distance * cosf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1)))},
			    i);
		}
	} else {
		actsPhase = End;
	}
}

void BlasterStandState::EndAct() { isFinish = true; }
