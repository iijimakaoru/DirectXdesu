#include "StageStart.h"
#include "Player.h"
#include "RailCamera.h"

void (StageStart::*StageStart::phase[])() = {
    &StageStart::LookDownPhase,
    &StageStart::TopRightPhase,
	&StageStart::BackPhase,
    &StageStart::CenterPhase,
	&StageStart::ComePhase,
	&StageStart::ExitPhase
};

StageStart::StageStart() {
	input_ = KInput::GetInstance();
	gameManager_ = GameManager::GetInstance();
	SetIsFinish(false);
	BaseMovie::Init();
	cameraPhase = LookDown;
}

void StageStart::Update() {
	// 関数テーブル更新
	(this->*phase[cameraPhase])();

	// スキップ
	if (cameraPhase < Exit) {
		if (input_->GetPadButtonDown(XINPUT_GAMEPAD_START) || gameManager_->GetIsStartMovie()) {
			cameraPhase = Exit;
		}
	}
}

void StageStart::LookDownPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	phaseTime_ = 180.0f;

	if (phaseTimer_ == 0) {
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

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;

		const float startPosY = 20.0f;
		const float endPosY = 10.0f;

		const float startPosZ = 40.0f;
		const float endPosZ = 45.0f;

		camera->SetCameraPos(
		    {camera->GetCameraPos().x,
		     MyEase::Lerp(
		         player->GetWorldPos().y + startPosY, player->GetWorldPos().y + endPosY,
		         phaseTimer_ / phaseTime_),
		     MyEase::Lerp(
		         player->GetWorldPos().z + startPosZ, player->GetWorldPos().z + endPosZ,
		         phaseTimer_ / phaseTime_)});

		const float startRotX = 20.0f;
		const float endRotX = 10.0f;

		camera->SetCameraRot(
		    {MyEase::Lerp(startRotX, endRotX, phaseTimer_ / phaseTime_), camera->GetCameraRot().y,
		     camera->GetCameraRot().z});
	} else {
		phaseTimer_ = 0;
		cameraPhase = TopRight;
	}
}

void StageStart::TopRightPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	phaseTime_ = 180.0f;

	if (phaseTimer_ == 0) {
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

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;

		const float startPosZ = 3.5f;
		const float endPosZ = 1.5f;

		camera->SetCameraPos(
		    {camera->GetCameraPos().x, camera->GetCameraPos().y,
		     MyEase::Lerp(
		         player->GetWorldPos().z + startPosZ, player->GetWorldPos().z + endPosZ,
		         phaseTimer_ / phaseTime_)});
	} else {
		phaseTimer_ = 0;
		cameraPhase = Back;
	}
}

void StageStart::BackPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	phaseTime_ = 180.0f;

	if (phaseTimer_ == 0) {
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

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;

		const float startPosX = 2.0f;
		const float endPosX = 1.5f;
		camera->SetCameraPos(
		    {MyEase::Lerp(
		         player->GetWorldPos().x - startPosX, player->GetWorldPos().x - endPosX,
		         phaseTimer_ / phaseTime_),
		     camera->GetCameraPos().y, camera->GetCameraPos().z});

		const float startRotY = 35.0f;
		const float endRotY = 27.5f;
		camera->SetCameraRot(
		    {camera->GetCameraRot().x, MyEase::Lerp(startRotY, endRotY, phaseTimer_ / phaseTime_),
		     camera->GetCameraRot().z});
	} else {
		phaseTimer_ = 0;
		cameraPhase = Center;
	}
}

void StageStart::CenterPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	phaseTime_ = 120.0f;

	if (phaseTimer_ == 0) {
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

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;

		// ムービーバーを上下へ
		MovieBarOut(phaseTimer_ / phaseTime_);

		const float startPosZ = 10.0f;
		const float endPosZ = 30.0f;

		camera->SetCameraPos(
		    {camera->GetCameraPos().x, camera->GetCameraPos().y,
		     MyEase::OutCubicFloat(
		         player->GetWorldPos().z + startPosZ, player->GetWorldPos().z + endPosZ,
		         phaseTimer_ / phaseTime_)});
	} else {
		phaseTimer_ = 0;
		cameraPhase = Come;
	}
}

void StageStart::ComePhase() {
	Player* player = Player::nowPlayer;

	phaseTime_ = 60;

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;

		const float startPosZ = 50.0f;
		const float endPosZ = 100.0f;

		player->SetPos(
		    {player->GetPosition().x, player->GetPosition().y,
		     MyEase::OutCubicFloat(startPosZ, endPosZ, phaseTimer_ / phaseTime_)});
	} else {
		phaseTimer_ = 0;
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
