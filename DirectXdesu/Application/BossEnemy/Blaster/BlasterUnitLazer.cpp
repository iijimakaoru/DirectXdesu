#include "BlasterUnitLazer.h"
#include "Blaster.h"
#include "BlasterStandState.h"
#include "Ease.h"
#include "MyMath.h"

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

	if (actTimer == 0) {
		CubeSet();
	}

	if (actTimer < actTime) {
		actTimer++;
		for (size_t i = 0; i < 8; i++) {
			blaster->SetUnitsScale(
			    MyEase::InCubicVec3({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, actTimer / actTime), i);
		}
	} else {
		actTimer = 0;
		actsPhase = GoCube;
	}
}

void BlasterUnitLazer::CubeSet() {
	Blaster* blaster = Blaster::nowBlaster;

	uint32_t minForm = 0;
	uint32_t maxForm = 2;
	uint32_t formNum = MyMathUtility::GetRandI(minForm, maxForm);

	switch (formNum) {
	case 0:
		cubeForm = CubeForm::Alpha;
		break;
	case 1:
		cubeForm = CubeForm::Alpha;
		// cubeForm = CubeForm::Beta;
		break;
	case 2:
		cubeForm = CubeForm::Alpha;
		// cubeForm = CubeForm::Gamma;
		break;
	default:
		cubeForm = CubeForm::Alpha;
		// cubeForm = CubeForm::Gamma;
		break;
	}

	switch (cubeForm) {
	case BlasterUnitLazer::Alpha:
		for (size_t i = 0; i < 8; i++) {
			float sixteen = 8.0f;
			KMyMath::Vector2 setPos[8];
			setPos[0] = {sixteen / 2, sixteen / 2};
			setPos[1] = {-sixteen / 2, sixteen / 2};
			setPos[2] = {-sixteen / 2, -sixteen / 2};
			setPos[3] = {sixteen / 2, -sixteen / 2};
			setPos[4] = {sixteen, sixteen};
			setPos[5] = {-sixteen, sixteen};
			setPos[6] = {-sixteen, -sixteen};
			setPos[7] = {sixteen, -sixteen};

			blaster->SetUnitsPos({setPos[i].x, setPos[i].y, -12}, i);
		}
		break;
	case BlasterUnitLazer::Beta:
		for (size_t i = 0; i < 8; i++) {
			float sixteen = 8.0f;
			KMyMath::Vector2 setPos[8];
			setPos[0] = {sixteen / 2, sixteen / 2};
			setPos[1] = {-sixteen / 2, sixteen / 2};
			setPos[2] = {-sixteen / 2, -sixteen / 2};
			setPos[3] = {sixteen / 2, -sixteen / 2};
			setPos[4] = {sixteen, sixteen};
			setPos[5] = {-sixteen, sixteen};
			setPos[6] = {-sixteen, -sixteen};
			setPos[7] = {sixteen, -sixteen};

			blaster->SetUnitsPos({setPos[i].x, setPos[i].y, -12}, i);
		}
		break;
	case BlasterUnitLazer::Gamma:
		for (size_t i = 0; i < 8; i++) {
			float sixteen = 8.0f;
			KMyMath::Vector2 setPos[8];
			setPos[0] = {sixteen / 2, sixteen / 2};
			setPos[1] = {-sixteen / 2, sixteen / 2};
			setPos[2] = {-sixteen / 2, -sixteen / 2};
			setPos[3] = {sixteen / 2, -sixteen / 2};
			setPos[4] = {sixteen, sixteen};
			setPos[5] = {-sixteen, sixteen};
			setPos[6] = {-sixteen, -sixteen};
			setPos[7] = {sixteen, -sixteen};

			blaster->SetUnitsPos({setPos[i].x, setPos[i].y, -12}, i);
		}
		break;
	default:
		break;
	}
}

void BlasterUnitLazer::GoCubeAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 30.0f;

	if (actTimer < actTime) {
		actTimer++;
		blaster->SetUnitsPos(
		    MyEase::Lerp3D(
		        {blaster->GetUnitsPos(unitCount).x, blaster->GetUnitsPos(unitCount).y, -12},
		        {blaster->GetUnitsPos(unitCount).x, blaster->GetUnitsPos(unitCount).y, -200},
		        actTimer / actTime),
		    unitCount);
	} else {
		actTimer = 0;
		unitCount++;
		if (unitCount >= 8) {
			actsPhase = End;
		}
	}
}

void BlasterUnitLazer::EndAct() {
	Blaster::blasterActState = std::make_unique<BlasterStandState>();
}
