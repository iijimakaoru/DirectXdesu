#include "RailCamera.h"
#include "MyMath.h"
#include "Player.h"
#include "Ease.h"

const float RailCamera::moveSpeedPlayerMagnification = 8.0f;

const float RailCamera::advanceSpeed = 0.5f;

void RailCamera::Init(Player* player_, const KMyMath::Vector3& startPos)
{
	Camera::Init();

	cameraObject->transform.pos = startPos;
	cameraObject->transform.rot = { 0,0,0 };

	cameraObject->TransUpdate();

	viewProjection->SetMatView(MyMathUtility::MakeInverse(cameraObject->transform.matWorld));

	isCrash = false;

	isStageClear = false;

	player = player_;

	Camera::Update();
}

void RailCamera::Update()
{
	moveLimitMax = Player::GetPosLimitMax();
	moveLimitMin = Player::GetPosLimitMin();

	if (isCrash)
	{
		Crash();
	}
	else if (isStageClear)
	{

	}
	else
	{
		SetRot();

		Move();
	}

	cameraObject->TransUpdate();

	viewProjection->SetMatView(MyMathUtility::MakeInverse(cameraObject->transform.matWorld));

	Camera::Update();
}

void RailCamera::Move()
{
	// 移動速度
	KMyMath::Vector3 velocity;
	// カメラが傾いてる角度へ移動
	const float moveSpeed = Player::GetMoveSpeed() * moveSpeedPlayerMagnification;
	const KMyMath::Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (cameraObject->transform.rot.y / rotLimit.y);
	velocity.y = moveSpeed * -(cameraObject->transform.rot.x / rotLimit.x);

	if (isAdvance)
	{
		velocity.z = advanceSpeed;
	}

	// 移動
	cameraObject->transform.pos += velocity;

	// 移動限界から動くな
	cameraObject->transform.pos.x = max(cameraObject->transform.pos.x, moveLimitMin.x);
	cameraObject->transform.pos.x = min(cameraObject->transform.pos.x, moveLimitMax.x);
	cameraObject->transform.pos.y = max(cameraObject->transform.pos.y, moveLimitMin.y);
	cameraObject->transform.pos.y = min(cameraObject->transform.pos.y, moveLimitMax.y);
}

void RailCamera::Crash()
{
	const KMyMath::Vector3 playerDistance = { 40.0f, 0.0f, 40.0f }; //自機とカメラの距離

	// カメラの場所
	const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

	// 角度
	cameraObject->transform.rot.y = -135.0f;

	// カメラ動け
	cameraObject->transform.pos = crashCameraPos;
}

void RailCamera::SetRot()
{
	// 回転
	const KMyMath::Vector3 PlayerRotDivNum = { 5,5,8 };
	cameraObject->transform.rot.x = player->GetRot().x / PlayerRotDivNum.x;
	cameraObject->transform.rot.y = player->GetRot().y / PlayerRotDivNum.y;
	cameraObject->transform.rot.z = -player->GetRot().y / PlayerRotDivNum.z;
}

const float RailCamera::GetSpeed() const
{
	return advanceSpeed;
}

void RailCamera::CallCrash()
{
	// 墜落状態にする
	isCrash = true;
}
