#include "StageStart.h"
#include "RailCamera.h"
#include "Player.h"


StageStart::StageStart() {
	input_ = KInput::GetInstance();
	gameManager_ = GameManager::GetInstance();
	SetIsFinish(false);
	cameraPhase = LookDown;
}

void StageStart::Update() {
	switch (cameraPhase) {
	case StageStart::LookDown:
		LookDownPhase();
		break;
	case StageStart::TopRight:
		TopRightPhase();
		break;
	case StageStart::Back:
		BackPhase();
		break;
	case StageStart::Center:
		CenterPhase();
		break;
	case StageStart::Come:
		ComePhase();
		break;
	case StageStart::Exit:
		ExitPhase();
		break;
	default:
		break;
	}
}

void StageStart::LookDownPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	if (input_->GetPadButtonDown(XINPUT_GAMEPAD_START) || gameManager_->GetIsStartMovie()) {
		cameraPhase = Exit;
	}

	phaseTime = 180.0f;

	if (phaseTimer == 0) {
		MovieBarInInit();

		// 自機とカメラの距離
		const KMyMath::Vector3 playerDistance = {0.0f, 20.0f, 40.0f};

		// カメラの場所
		const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

		// 角度
		const float rotX = 20.0f;
		const float rotY = 180.0f;
		camera->SetCameraRot({rotX, rotY, camera->GetCameraRot().z});

		// カメラ動け
		camera->SetCameraPos(crashCameraPos);
	}

	if (phaseTimer < phaseTime) {
		phaseTimer++;

		const float startPosY = 20.0f;
		const float endPosY = 10.0f;

		const float startPosZ = 40.0f;
		const float endPosZ = 45.0f;

		camera->SetCameraPos(
		    {camera->GetCameraPos().x,
		     MyEase::Lerp(
		         player->GetWorldPos().y + startPosY, player->GetWorldPos().y + endPosY,
		         phaseTimer / phaseTime),
		     MyEase::Lerp(
		         player->GetWorldPos().z + startPosZ, player->GetWorldPos().z + endPosZ,
		         phaseTimer / phaseTime)});

		const float startRotX = 20.0f;
		const float endRotX = 10.0f;

		camera->SetCameraRot(
		    {MyEase::Lerp(startRotX, endRotX, phaseTimer / phaseTime),
		     camera->GetCameraRot().y, camera->GetCameraRot().z});
	} else {
		phaseTimer = 0;
		cameraPhase = TopRight;
	}
}

void StageStart::TopRightPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	if (input_->GetPadButtonDown(XINPUT_GAMEPAD_START) || gameManager_->GetIsStartMovie()) {
		cameraPhase = Exit;
	}

	phaseTime = 180.0f;

	if (phaseTimer == 0) {
		// 自機とカメラの距離
		const KMyMath::Vector3 playerDistance = {2.5f, 2.5f, 3.5f};

		// カメラの場所
		const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

		// 角度
		const float rotX = 45.0f;
		const float rotY = 250.0f;
		camera->SetCameraRot({rotX, rotY, camera->GetCameraRot().z});

		// カメラ動け
		camera->SetCameraPos(crashCameraPos);
	}

	if (phaseTimer < phaseTime) {
		phaseTimer++;

		const float startPosZ = 3.5f;
		const float endPosZ = 1.5f;

		camera->SetCameraPos(
		    {camera->GetCameraPos().x, camera->GetCameraPos().y,
		     MyEase::Lerp(
		         player->GetWorldPos().z + startPosZ, player->GetWorldPos().z + endPosZ,
		         phaseTimer / phaseTime)});
	} else {
		phaseTimer = 0;
		cameraPhase = Back;
	}
}

void StageStart::BackPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	if (input_->GetPadButtonDown(XINPUT_GAMEPAD_START) || gameManager_->GetIsStartMovie()) {
		cameraPhase = Exit;
	}

	phaseTime = 180.0f;

	if (phaseTimer == 0) {
		// 自機とカメラの距離
		const KMyMath::Vector3 playerDistance = {-2.0f, -1.8f, -3.5f};

		// カメラの場所
		const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

		// 角度
		const float rotX = -35.0f;
		const float rotY = 27.5f;
		camera->SetCameraRot({rotX, rotY, camera->GetCameraRot().z});

		// カメラ動け
		camera->SetCameraPos(crashCameraPos);
	}

	if (phaseTimer < phaseTime) {
		phaseTimer++;

		const float startPosX = 2.0f;
		const float endPosX = 1.5f;
		camera->SetCameraPos(
		    {MyEase::Lerp(
		         player->GetWorldPos().x - startPosX, player->GetWorldPos().x - endPosX,
		         phaseTimer / phaseTime),
		     camera->GetCameraPos().y, camera->GetCameraPos().z});

		const float startRotY = 35.0f;
		const float endRotY = 27.5f;
		camera->SetCameraRot(
		    {camera->GetCameraRot().x,
		     MyEase::Lerp(startRotY, endRotY, phaseTimer / phaseTime),
		     camera->GetCameraRot().z});
	} else {
		phaseTimer = 0;
		cameraPhase = Center;
	}
}

void StageStart::CenterPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	if (input_->GetPadButtonDown(XINPUT_GAMEPAD_START) || gameManager_->GetIsStartMovie()) {
		cameraPhase = Exit;
	}

	phaseTime = 120.0f;

	if (phaseTimer == 0) {
		// 自機とカメラの距離
		const KMyMath::Vector3 playerDistance = {0.0f, 1.0f, 10.0f};

		// カメラの場所
		const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

		// 角度
		const float rotX = 0.0f;
		const float rotY = 180.0f;
		camera->SetCameraRot({rotX, rotY, camera->GetCameraRot().z});

		// カメラ動け
		camera->SetCameraPos(crashCameraPos);
	}

	if (phaseTimer < phaseTime) {
		phaseTimer++;

		// ムービーバーを上下へ
		MovieBarOut(phaseTimer / phaseTime);

		const float startPosZ = 10.0f;
		const float endPosZ = 30.0f;

		camera->SetCameraPos(
		    {camera->GetCameraPos().x, camera->GetCameraPos().y,
		     MyEase::OutCubicFloat(
		         player->GetWorldPos().z + startPosZ, player->GetWorldPos().z + endPosZ,
		         phaseTimer / phaseTime)});
	} else {
		phaseTimer = 0;
		cameraPhase = Come;
	}
}

void StageStart::ComePhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	if (input_->GetPadButtonDown(XINPUT_GAMEPAD_START) || gameManager_->GetIsStartMovie()) {
		cameraPhase = Exit;
	}

	phaseTime = 60;

	if (phaseTimer < phaseTime) {
		phaseTimer++;

		const float startPosZ = 50.0f;
		const float endPosZ = 100.0f;

		player->SetPos(
		    {player->GetPosition().x, player->GetPosition().y,
		     MyEase::OutCubicFloat(startPosZ, endPosZ, phaseTimer / phaseTime)});
	} else {
		phaseTimer = 0;
		cameraPhase = Exit;
	}
}

void StageStart::ExitPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	MovieBarOutInit();
	camera->EndStart();
	player->EndStart();
	Player::isStartEase = true;
	// 親子関係接続
	player->SetParent(&camera->GetTransform());
	SetIsFinish(true);
}
