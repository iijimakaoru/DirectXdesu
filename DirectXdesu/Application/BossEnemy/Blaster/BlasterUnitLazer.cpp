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
	case BlasterUnitLazer::GoPoint:
		GoPointAct();
		break;
	case BlasterUnitLazer::CubeOpen:
		CubeOpenAct();
		break;
	case BlasterUnitLazer::Wait:
		WaitAct();
		break;
	case BlasterUnitLazer::GoCube:
		GoCubeAct();
		break;
	case BlasterUnitLazer::BackPos:
		BackPosAct();
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
		actsPhase = GoPoint;
	}
}

void BlasterUnitLazer::GoPointAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 60.0f;

	if (actTimer < actTime) {
		KMyMath::Vector3 move =
		    MyEase::OutCubicVec3(blaster->GetWorldPos(), {0, 5.0f, 650.0f}, actTimer / actTime);

		blaster->SetPos(move);

		actTimer++;
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
		LazerShot();
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
		if (stayTimer < stayTime) {
			stayTimer++;
			BulletManager::GetInstance()->UnitLazerCall();
			for (size_t i = 0; i < 16; i++) {
				BulletManager::GetInstance()->LazerOpen(stayTimer / stayTime, i);
			}
			LazerTrack();
		} else {
			isStay = false;
			actsPhase = Wait;
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
			float sixteen = 18.0f;
			KMyMath::Vector2 setPos[8];
			setPos[0] = {0, 0};
			setPos[1] = {-sixteen / 2, sixteen / 2};
			setPos[2] = {sixteen / 2, sixteen / 2};
			setPos[3] = {0, -sixteen / 2};
			setPos[4] = {sixteen, sixteen};
			setPos[5] = {-sixteen, sixteen};
			setPos[6] = {-sixteen, -sixteen};
			setPos[7] = {sixteen, -sixteen};

			blaster->SetUnitsPos({setPos[i].x, setPos[i].y, -12}, i);
		}
		break;
	case BlasterUnitLazer::Beta:
		for (size_t i = 0; i < 8; i++) {
			float sixteen = 18.0f;
			KMyMath::Vector2 setPos[8];
			setPos[0] = {0.0f, sixteen / 2};
			setPos[1] = {-sixteen, sixteen};
			setPos[2] = {-sixteen / 2, 0.0f};
			setPos[3] = {-sixteen, -sixteen};
			setPos[4] = {0.0f, -sixteen / 2};
			setPos[5] = {sixteen, -sixteen};
			setPos[6] = {sixteen / 2, 0.0f};
			setPos[7] = {sixteen, sixteen};

			blaster->SetUnitsPos({setPos[i].x, setPos[i].y, -12}, i);
		}
		break;
	case BlasterUnitLazer::Gamma:
		for (size_t i = 0; i < 8; i++) {
			float sixteen = 18.0f;
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
			actsPhase = BackPos;
		}
	}
}

void BlasterUnitLazer::WaitAct() {
	actTime = 60.0f;

	if (actTimer < actTime) {
		actTimer++;
	} else {
		actTimer = 0;
		actsPhase = GoCube;
	}
}

void BlasterUnitLazer::BackPosAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 60.0f;

	if (actTimer < actTime) {
		KMyMath::Vector3 move =
		    MyEase::OutCubicVec3(blaster->GetWorldPos(), {0, 20, 650}, actTimer / actTime);

		blaster->SetPos(move);

		actTimer++;
	} else {
		actTimer = 0;
		actsPhase = End;
	}
}

void BlasterUnitLazer::EndAct() {
	BulletManager::GetInstance()->UnitLazerDelete();
	Blaster::blasterActState = std::make_unique<BlasterStandState>();
}

void BlasterUnitLazer::LazerShot() {
	Blaster* blaster = Blaster::nowBlaster;

	// ワールド座標
	for (uint32_t i = 0; i < 8; i++) {
		KMyMath::Vector3 ePos = blaster->UnitsGetWorldPos(i);

		uint32_t j = i + 8;

		BulletManager::GetInstance()->UnitLazerSet(ePos, {0, 0, 0.0f}, i);
		BulletManager::GetInstance()->UnitLazerSet(ePos, {0, 0, 90.0f}, j);
	}
}

void BlasterUnitLazer::LazerTrack() {
	Blaster* blaster = Blaster::nowBlaster;

	// ワールド座標
	for (uint32_t i = 0; i < 8; i++) {
		KMyMath::Vector3 ePos = blaster->UnitsGetWorldPos(i);

		uint32_t j = i + 8;

		BulletManager::GetInstance()->UnitLazerSet(ePos, {0, 0, 0.0f}, i);
		BulletManager::GetInstance()->UnitLazerSet(ePos, {0, 0, 90.0f}, j);
	}
}
