#include "BlasterAimState.h"
#include "Blaster.h"
#include "BlasterStandState.h"
#include "BulletManager.h"
#include "Ease.h"
#include "Player.h"

BlasterAimState::BlasterAimState() {
	audioManager = AudioManager::GetInstance();
	actsPhase = STAND;
}

void BlasterAimState::Update() {
	switch (actsPhase) {
	case STAND:
		StandAct();
		break;
	case CHARGE:
		ChargeAct();
		break;
	case SHOT:
		ShotAct();
		break;
	case END:
		EndAct();
		break;
	default:
		break;
	}
}

void BlasterAimState::BulletShot() {
	Player* player = Player::nowPlayer;

	Blaster* blaster = Blaster::nowBlaster;

	// 弾の速度
	const float kBulletSpeed = 2.5f;

	// 弾のパワー
	const float bulletPower = 5.0f;

	// 自キャラのワールド座標
	KMyMath::Vector3 pPos = player->GetWorldPos();

	// ワールド座標
	KMyMath::Vector3 ePos = {
	    blaster->GetWorldPos().x, blaster->GetWorldPos().y, blaster->GetWorldPos().z - 8.0f};

	// 差分ベクトル
	KMyMath::Vector3 vec = pPos - ePos;

	// 弾生成
	BulletManager::GetInstance()->EnemyBulletShot(ePos, vec, {1, 1, 1}, kBulletSpeed, bulletPower);
}

void BlasterAimState::StandAct() {
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
		actsPhase = CHARGE;
	}
}

void BlasterAimState::ChargeAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 240.0f;
	startTime = 15.0f;

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

		if (actTimer == 0) {
			audioManager->SEPlay_wav("chargeSE.wav", 0.5f);
		}

		actTimer++;

		angleSpeed = MyEase::OutCubicFloat(0.0f, 12.0f, actTimer / actTime);

		angle += angleSpeed;

		// 360を超えたら
		if (angle >= 360) {
			angle = 0;
		}

		const float radian = DirectX::XMConvertToRadians(angle);
		float distance = 8.0f;

		for (size_t i = 0; i < 8; i++) {
			if (i > 3) {
				distance = 10.0f;
			}
			blaster->SetUnitsPos(
			    {distance * sinf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     distance * cosf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     blaster->GetUnitsPos(i).z},
			    i);
		}
	} else {
		actTimer = 0;
		actsPhase = SHOT;
	}
}

void BlasterAimState::ShotAct() {
	Blaster* blaster = Blaster::nowBlaster;

	actTime = 240.0f;

	if (actTimer < actTime) {
		actTimer++;

		angle += 12.0f;

		// 360を超えたら
		if (angle >= 360) {
			angle = 0;
		}

		const float radian = DirectX::XMConvertToRadians(angle);
		float distance = 8.0f;

		for (size_t i = 0; i < 8; i++) {
			if (i > 3) {
				distance = 10.0f;
			}

			blaster->SetUnitsPos(
			    {distance * sinf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     distance * cosf(radian + DirectX::XMConvertToRadians(90.0f * (i + 1))),
			     blaster->GetUnitsPos(i).z},
			    i);
		}

		BulletShot();
	} else {
		actTimer = 0;
		actsPhase = END;
	}
}

void BlasterAimState::EndAct() { Blaster::blasterActState = std::make_unique<BlasterStandState>(); }
