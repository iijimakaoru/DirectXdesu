#include "RailCamera.h"
#include "Ease.h"
#include "MyMath.h"
#include "Player.h"

RailCamera* RailCamera::nowRailCamera = nullptr;

const float RailCamera::moveSpeedPlayerMagnification = 1.0f;

const float RailCamera::advanceSpeed = 0.5f;

void RailCamera::Init(Player* player_) {
	Camera::Init();

	cameraTransform.TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraTransform.GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	isCrash = false;

	isStageClear = false;

	player = player_;

	Camera::Update();
}

void RailCamera::Update(bool isStart_, bool isBossMovie_, bool isClearMovie_) {
	moveLimitMax = {Player::GetPosLimitMax().x * 2/3, Player::GetPosLimitMax().y / 2};
	moveLimitMin = {Player::GetPosLimitMin().x * 2/3, Player::GetPosLimitMin().y / 2};

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
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

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

void RailCamera::SetParent(const Transform* parent) { cameraTransform.SetParent(parent); }

void RailCamera::SetPosZ(const float& zPos) {
	cameraTransform.SetPos({cameraTransform.GetPos().x,
		cameraTransform.GetPos().y,
		zPos - 50.0f});
}

void RailCamera::CallCrash() {
	// 墜落状態にする
	isCrash = true;
}

void RailCamera::EndStart() {
	cameraTransform.SetPos({0, 0, 0});
	cameraTransform.SetRot({0, 0, 0});
}
