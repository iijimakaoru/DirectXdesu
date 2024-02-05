#include "BlasterUnitLazer.h"
#include "Blaster.h"
#include "BlasterStandState.h"
#include "Ease.h"

BlasterUnitLazer::BlasterUnitLazer() { actsPhase = CubeClose; }

void BlasterUnitLazer::Update() {
	switch (actsPhase) {
	case BlasterUnitLazer::CubeClose:
		CubeCloseAct();
		break;
	case BlasterUnitLazer::CubeOpen:
		CubeOpenAct();
		break;
	case BlasterUnitLazer::GoCube:
		GoCubeAct();
		break;
	case BlasterUnitLazer::End:
		EndAct();
		break;
	default:
		break;
	}
}

void BlasterUnitLazer::CubeCloseAct() {
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
		actsPhase = CubeOpen;
	}
}

void BlasterUnitLazer::CubeOpenAct() {
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
		actsPhase = GoCube;
	}
}

void BlasterUnitLazer::GoCubeAct() {}

void BlasterUnitLazer::EndAct() {
	Blaster::blasterActState = std::make_unique<BlasterStandState>();
}
