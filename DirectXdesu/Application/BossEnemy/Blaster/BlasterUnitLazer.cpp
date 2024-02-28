#include "BlasterUnitLazer.h"
#include "Blaster.h"
#include "BlasterStandState.h"
#include "BulletManager.h"
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
		BulletShot();
	}

	if (actTimer < actTime) {
		actTimer++;
		for (size_t i = 0; i < 8; i++) {
			blaster->SetUnitsScale(
			    MyEase::InCubicVec3({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, actTimer / actTime), i);
		}
	} else {
		isStay = true;
	}

	if (isStay) {
		LazerTrack();

		if (stayTimer < stayTime) {
			stayTimer++;
		} else {
			isStay = false;
			actsPhase = GoCube;
			actTimer = 0;
			stayTimer = 0;
		}
	}
}

void BlasterUnitLazer::CubeSet() {
	Blaster* blaster = Blaster::nowBlaster;

	uint32_t minForm = 0;
	uint32_t maxForm = 3;
	uint32_t formNum = MyMathUtility::GetRandI(minForm, maxForm);

	switch (formNum) {
	case 0:
		cubeForm = CubeForm::Alpha;
		break;
	case 1:
		cubeForm = CubeForm::Alpha;
		break;
	case 2:
		cubeForm = CubeForm::Beta;
		break;
	case 3:
		cubeForm = CubeForm::Gamma;
		break;
	default:
		cubeForm = CubeForm::Gamma;
		break;
	}

	switch (cubeForm) {
	case BlasterUnitLazer::Alpha:
		for (size_t i = 0; i < 8; i++) {
			float sixteen = 12.0f;
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
			float sixteen = 12.0f;
			KMyMath::Vector2 setPos[8];
			setPos[0] = {sixteen, sixteen / 2};
			setPos[1] = {-sixteen, sixteen / 2};
			setPos[2] = {-sixteen, -sixteen / 2};
			setPos[3] = {sixteen, -sixteen / 2};
			setPos[4] = {sixteen / 2, sixteen};
			setPos[5] = {-sixteen / 2, sixteen};
			setPos[6] = {-sixteen / 2, -sixteen};
			setPos[7] = {sixteen / 2, -sixteen};

			blaster->SetUnitsPos({setPos[i].x, setPos[i].y, -12}, i);
		}
		break;
	case BlasterUnitLazer::Gamma:
		for (size_t i = 0; i < 8; i++) {
			float sixteen = 12.0f;
			KMyMath::Vector2 setPos[8];
			setPos[0] = {sixteen / 2, 0.0f};
			setPos[1] = {-sixteen / 2, 0.0f};
			setPos[2] = {0.0f, -sixteen / 2};
			setPos[3] = {0.0f, sixteen / 2};
			setPos[4] = {0.0f, sixteen};
			setPos[5] = {0.0f, -sixteen};
			setPos[6] = {-sixteen, 0.0f};
			setPos[7] = {sixteen, 0.0f};

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

	LazerTrack();

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
	BulletManager::GetInstance()->UnitLazerDelete();
	Blaster::blasterActState = std::make_unique<BlasterStandState>();
}

void BlasterUnitLazer::BulletShot() {
	Blaster* blaster = Blaster::nowBlaster;

	// ワールド座標
	for (uint32_t i = 0; i < 8; i++) {
		KMyMath::Vector3 ePos = blaster->UnitsGetWorldPos(i);

		BulletManager::GetInstance()->UnitLazerSet(ePos, {0, 0, 90.0f});
	}
}

void BlasterUnitLazer::LazerTrack() {
	Blaster* blaster = Blaster::nowBlaster;

	// ワールド座標
	for (uint32_t i = 0; i < 8; i++) {
		KMyMath::Vector3 ePos = blaster->UnitsGetWorldPos(i);

		BulletManager::GetInstance()->UnitLaserTrack(ePos, {0, 0, 90.0f});
	}
}
