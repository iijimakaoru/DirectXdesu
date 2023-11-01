#include "RailCamera.h"
#include "MyMath.h"
#include "Player.h"
#include "Ease.h"

const float RailCamera::moveSpeedPlayerMagnification = 8.0f;

const float RailCamera::advanceSpeed = 0.5f;

void RailCamera::Init(Player* player_, const KMyMath::Vector3& startPos_)
{
	Camera::Init();

	startPos = startPos_;

	cameraObject->TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraObject->GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	isCrash = false;

	isStageClear = false;

	player = player_;

	Camera::Update();
}

void RailCamera::Update(bool isStart_)
{
	moveLimitMax = Player::GetPosLimitMax();
	moveLimitMin = Player::GetPosLimitMin();

	if (isStart_)
	{
		
	}
	else if (isCrash)
	{
		Crash();
	}
	else if (isStageClear)
	{

	}
	else
	{
		if (!Player::isStartEase)
		{
			SetRot();
		}

		Move();
	}

	cameraObject->TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraObject->GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	Camera::Update();
}

void RailCamera::Move()
{
	// 移動速度
	KMyMath::Vector3 velocity;
	// カメラが傾いてる角度へ移動
	const float moveSpeed = Player::GetMoveSpeed() * moveSpeedPlayerMagnification;
	const KMyMath::Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (cameraObject->GetRot().y / rotLimit.y);
	velocity.y = moveSpeed * -(cameraObject->GetRot().x / rotLimit.x);

	if (isAdvance)
	{
		velocity.z = advanceSpeed;
	}

	// 移動
	cameraObject->AddSetPos(velocity);

	// 移動限界から動くな
	cameraObject->SetPos({ max(cameraObject->GetPos().x, moveLimitMin.x),
		max(cameraObject->GetPos().y, moveLimitMin.y),
		cameraObject->GetPos().z });
	cameraObject->SetPos({ min(cameraObject->GetPos().x, moveLimitMax.x),
		min(cameraObject->GetPos().y, moveLimitMax.y),
		cameraObject->GetPos().z });
}

void RailCamera::Crash()
{
	//自機とカメラの距離
	const KMyMath::Vector3 playerDistance = { 40.0f, 0.0f, 40.0f };

	// カメラの場所
	const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

	// 角度
	cameraObject->SetRot({ 0.0f,-135.0f ,0.0f });

	// カメラ動け
	cameraObject->SetPos(crashCameraPos);
}

void RailCamera::SetRot()
{
	// 回転
	const KMyMath::Vector3 PlayerRotDivNum = { 5,5,8 };
	cameraObject->SetRot({ player->GetRot().x / PlayerRotDivNum.x ,
		player->GetRot().y / PlayerRotDivNum.y ,
		-player->GetRot().y / PlayerRotDivNum.z });
}

void RailCamera::SetIsAdvance(bool isAdvance_)
{
	isAdvance = isAdvance_;
}

const float RailCamera::GetSpeed()
{
	return advanceSpeed;
}

void RailCamera::CallCrash()
{
	// 墜落状態にする
	isCrash = true;
}

void RailCamera::EndStart()
{
	cameraObject->SetPos(startPos);
	cameraObject->SetRot({ 0,0,0 });
}
