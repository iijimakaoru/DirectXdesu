#include "RailCamera.h"
#include "Ease.h"
#include "MyMath.h"
#include "Player.h"

RailCamera* RailCamera::nowRailCamera = nullptr;

const float RailCamera::moveSpeedPlayerMagnification = 8.0f;

const float RailCamera::advanceSpeed = 0.5f;

void RailCamera::Init(Player* player_, const KMyMath::Vector3& startPos_) {
	Camera::Init();

	startPos = startPos_;

	cameraTransform.TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraTransform.GetMatWorld();
	matView = MyMathUtility::MakeInverse(nowMatWorld);

	isCrash = false;

	isStageClear = false;

	player = player_;

	Camera::Update();
}

void RailCamera::Update(bool isStart_, bool isBossMovie_, bool isClearMovie_) {
	moveLimitMax = Player::GetPosLimitMax();
	moveLimitMin = Player::GetPosLimitMin();

	if (isStart_) {

	} else if (isBossMovie_) {

	} else if (isClearMovie_) {

	} else if (isCrash) {
		Crash();
	} else if (isStageClear) {

	} else {
		if (!Player::isStartEase) {
			SetRot();
		}

		Move();
	}

	cameraTransform.TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraTransform.GetMatWorld();
	matView = MyMathUtility::MakeInverse(nowMatWorld);

	Camera::Update();
}

void RailCamera::Move() {
	// 移動速度
	KMyMath::Vector3 velocity;
	// カメラが傾いてる角度へ移動
	const float moveSpeed = Player::GetMoveSpeed() * moveSpeedPlayerMagnification;
	const KMyMath::Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (cameraTransform.GetRot().y / rotLimit.y);
	velocity.y = moveSpeed * -(cameraTransform.GetRot().x / rotLimit.x);

	if (isAdvance) {
		velocity.z = advanceSpeed;
	}

	// 移動
	cameraTransform.AddSetPos(velocity);

	// 移動限界から動くな
	cameraTransform.SetPos(
	    {max(cameraTransform.GetPos().x, moveLimitMin.x),
	     max(cameraTransform.GetPos().y, moveLimitMin.y), cameraTransform.GetPos().z});
	cameraTransform.SetPos(
	    {min(cameraTransform.GetPos().x, moveLimitMax.x),
	     min(cameraTransform.GetPos().y, moveLimitMax.y), cameraTransform.GetPos().z});
}

void RailCamera::Crash() {
	// 自機とカメラの距離
	const KMyMath::Vector3 playerDistance = {40.0f, 0.0f, 40.0f};

	// カメラの場所
	const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

	// 角度
	cameraTransform.SetRot({0.0f, -135.0f, 0.0f});

	// カメラ動け
	cameraTransform.SetPos(crashCameraPos);
}

void RailCamera::SetRot() {
	// 回転
	const KMyMath::Vector3 PlayerRotDivNum = {5, 5, 8};
	cameraTransform.SetRot(
	    {player->GetRot().x / PlayerRotDivNum.x, 
		player->GetRot().y / PlayerRotDivNum.y,
	     -player->GetRot().y / PlayerRotDivNum.z});
}

void RailCamera::SetIsAdvance(bool isAdvance_) { isAdvance = isAdvance_; }

const float RailCamera::GetSpeed() { return advanceSpeed; }

void RailCamera::SetParent(const Transform* parent) { cameraTransform.SetParent(parent); }

void RailCamera::CallCrash() {
	// 墜落状態にする
	isCrash = true;
}

void RailCamera::EndStart() {
	cameraTransform.SetPos(startPos);
	cameraTransform.SetRot({0, 0, 0});
}
