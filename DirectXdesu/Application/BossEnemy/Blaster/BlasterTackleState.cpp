#include "BlasterTackleState.h"
#include "Blaster.h"
#include "Ease.h"
#include "Player.h"

BlasterTackleState::BlasterTackleState() {}

void BlasterTackleState::Update() {
	Blaster* blaster = Blaster::nowBlaster;
	// 予備動作
	if (actPhase == 0) {
		actTime = 15.0f;

		if (actTimer < actTime) {
			actTimer++;
			for (size_t i = 0; i < 8; i++) {
				blaster->SetUnitsScale(
				    MyEase::InCubicVec3({1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, actTimer / actTime),
				    i);
			}
		} else {
			actTimer = 0;
			actPhase++;
		}
	}
	// キューブ展開
	else if (actPhase == 1) {
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
			actPhase++;
		}
	}
	// 突進前行動(後ろに移動)
	else if (actPhase == 2) {
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
			actPhase++;
		}
	}
	// 突進準備
	else if (actPhase == 3) {
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
			actPhase++;
		}
	}
	// 突進
	else if (actPhase == 4) {
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
			actPhase++;
		}
	}
	// 定位置へ
	else if (actPhase == 5) {
		actTime = 90.0f;

		if (actTimer < actTime) {
			angle = 0;

			KMyMath::Vector3 move =
			    MyEase::OutCubicVec3({0, 50.0f, 650.0f}, {0, 0, 650}, actTimer / actTime);

			blaster->SetPos(move);

			actTimer++;
		} else {
			actTimer = 0;
			actPhase++;
		}
	}
	// 終わり
	else {
		isFinish = true;
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
}

void BlasterTackleState::Tackle() {
	Blaster* blaster = Blaster::nowBlaster;

	// タックルの速度
	const float tackleSpeed = 10.0f;

	blaster->AddSetPos(MyMathUtility::MakeNormalize(tackleVec) * tackleSpeed);
}
