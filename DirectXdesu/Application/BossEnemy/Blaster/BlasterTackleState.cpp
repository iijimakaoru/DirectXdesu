#include "BlasterTackleState.h"
#include "Blaster.h"
#include "BlasterStandState.h"
#include "Ease.h"
#include "Player.h"

BlasterTackleState::BlasterTackleState() { actsPhase = CubeClose; }

void BlasterTackleState::Update() {
	switch (actsPhase) {
	case BlasterTackleState::CubeClose:
		CubeCloseAct();
		break;
	case BlasterTackleState::CubeOpen:
		CubeOpenAct();
		break;
	case BlasterTackleState::Aim:
		AimAct();
		break;
	case BlasterTackleState::StandTackle:
		StandTackleAct();
		break;
	case BlasterTackleState::GoTackle:
		GoTackleAct();
		break;
	case BlasterTackleState::BackPos:
		BackPosAct();
		break;
	case BlasterTackleState::End:
		EndAct();
		break;
	default:
		break;
	}
}

void BlasterTackleState::TackleVec() {
	Player* player = Player::nowPlayer;

	Blaster* blaster = Blaster::nowBlaster;

	// 自キャラのワールド座標
	KMyMath::Vector3 pPos = player->GetWorldPos();

	// ワールド座標
	KMyMath::Vector3 ePos = blaster->GetWorldPos();

	// 差分ベクトル
	tackleVec = pPos - ePos;

	// 2Dレティクルの更新
	blaster->reticle2d->Update(blaster->viewPro, pPos);
}

void BlasterTackleState::Tackle() {
	Blaster* blaster = Blaster::nowBlaster;

	// タックルの速度
	const float tackleSpeed = 10.0f;

	blaster->AddSetPos(MyMathUtility::MakeNormalize(tackleVec) * tackleSpeed);
}

void BlasterTackleState::CubeCloseAct() {
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

void BlasterTackleState::CubeOpenAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 60.0f;
	startTime = 15.0f;

	// キューブ出現
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
			float posZ = -3.0f;
			for (size_t i = 0; i < 8; i++) {
				if (i > 3) {
					posZ = -12.0f;
				}
				blaster->SetUnitsPos(
				    {blaster->GetUnitsPos(i).x, blaster->GetUnitsPos(i).y, posZ}, i);
			}
			angle = 45.0f;
		}

		actTimer++;

		angle += 2.5f;

		// 360を超えたら
		if (angle >= 360) {
			angle = 0;
		}

		const float radian = DirectX::XMConvertToRadians(angle);
		float distance = 8.0f;

		for (size_t i = 0; i < 8; i++) {
			if (i > 3) {
				distance = 5.0f;
			}
			blaster->SetUnitsPos(
			    {distance * sinf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     distance * cosf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     blaster->GetUnitsPos(i).z},
			    i);
		}
	} else {
		actTimer = 0;
		startTimer = 0;
		actsPhase = Aim;
	}
}

void BlasterTackleState::AimAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 180.0f;

	// 照準合わせ
	TackleVec();

	if (actTimer < actTime) {
		KMyMath::Vector3 move =
		    MyEase::OutCubicVec3(blaster->GetWorldPos(), {0, 0, 770}, actTimer / actTime);

		blaster->SetPos(move);

		float posZ = -3.0f;
		if (actTimer == 0) {
			for (size_t i = 0; i < 8; i++) {
				if (i > 3) {
					posZ = -12.0f;
				}
				blaster->SetUnitsPos(
				    {blaster->GetUnitsPos(i).x, blaster->GetUnitsPos(i).y, posZ}, i);
			}
			angle = 45.0f;
		}

		actTimer++;

		angle += 15.0;

		// 360を超えたら
		if (angle >= 360) {
			angle = 0;
		}

		const float radian = DirectX::XMConvertToRadians(angle);
		float distance = 8.0f;

		for (size_t i = 0; i < 8; i++) {
			if (i > 3) {
				distance = 5.0f;
			}
			blaster->SetUnitsPos(
			    {distance * sinf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     distance * cosf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     blaster->GetUnitsPos(i).z},
			    i);
		}
	} else {
		actTimer = 0;
		actsPhase = StandTackle;
	}
}

void BlasterTackleState::StandTackleAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 30.0f;

	if (actTimer < actTime) {
		float posZ = -3.0f;
		if (actTimer == 0) {
			for (size_t i = 0; i < 8; i++) {
				if (i > 3) {
					posZ = -12.0f;
				}
				blaster->SetUnitsPos(
				    {blaster->GetUnitsPos(i).x, blaster->GetUnitsPos(i).y, posZ}, i);
			}
			angle = 45.0f;
		}

		actTimer++;

		angle += 15.0;

		// 360を超えたら
		if (angle >= 360) {
			angle = 0;
		}

		const float radian = DirectX::XMConvertToRadians(angle);
		float distance = 8.0f;

		for (size_t i = 0; i < 8; i++) {
			if (i > 3) {
				distance = 5.0f;
			}
			blaster->SetUnitsPos(
			    {distance * sinf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     distance * cosf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     blaster->GetUnitsPos(i).z},
			    i);
		}
	} else {
		actTimer = 0;
		actsPhase = GoTackle;
	}
}

void BlasterTackleState::GoTackleAct() {
	Blaster* blaster = Blaster::nowBlaster;

	blaster->SetIsMuteki(true);

	actTime = 90.0f;

	if (actTimer < actTime) {
		Tackle();

		float posZ = -3.0f;
		if (actTimer == 0) {
			for (size_t i = 0; i < 8; i++) {
				if (i > 3) {
					posZ = -12.0f;
				}
				blaster->SetUnitsPos(
				    {blaster->GetUnitsPos(i).x, blaster->GetUnitsPos(i).y, posZ}, i);
			}
			angle = 45.0f;
		}

		actTimer++;

		angle += 15.0;

		// 360を超えたら
		if (angle >= 360) {
			angle = 0;
		}

		const float radian = DirectX::XMConvertToRadians(angle);
		float distance = 8.0f;

		for (size_t i = 0; i < 8; i++) {
			if (i > 3) {
				distance = 5.0f;
			}
			blaster->SetUnitsPos(
			    {distance * sinf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     distance * cosf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     blaster->GetUnitsPos(i).z},
			    i);
		}
	} else {
		actTimer = 0;
		actsPhase = BackPos;
	}
}

void BlasterTackleState::BackPosAct() {
	Blaster* blaster = Blaster::nowBlaster;
	blaster->isReticle = false;

	blaster->SetIsMuteki(false);

	actTime = 90.0f;

	if (actTimer < actTime) {
		angle = 0;

		KMyMath::Vector3 move =
		    MyEase::OutCubicVec3({0, 100.0f, 650.0f}, {0, 20, 650}, actTimer / actTime);

		blaster->SetPos(move);

		actTimer++;
	} else {
		actTimer = 0;
		actsPhase = End;
	}
}

void BlasterTackleState::EndAct() {
	Blaster::blasterActState = std::make_unique<BlasterStandState>();
}
