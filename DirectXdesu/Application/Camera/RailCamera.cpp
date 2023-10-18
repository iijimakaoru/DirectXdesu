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

	isStart = true;

	Camera::Update();
}

void RailCamera::Update()
{
	moveLimitMax = Player::GetPosLimitMax();
	moveLimitMin = Player::GetPosLimitMin();

	if (isStart)
	{
		Start();
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
	//自機とカメラの距離
	const KMyMath::Vector3 playerDistance = { 40.0f, 0.0f, 40.0f };

	// カメラの場所
	const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

	// 角度
	cameraObject->transform.rot.y = -135.0f;

	// カメラ動け
	cameraObject->transform.pos = crashCameraPos;
}

void RailCamera::Start()
{
	// カメラワーク一段階(上から見下ろし)
	if (startPhase == 0)
	{
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0)
		{
			//自機とカメラの距離
			const KMyMath::Vector3 playerDistance = { 0.0f, 20.0f, 40.0f };

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			cameraObject->transform.rot.x = 20.0f;
			cameraObject->transform.rot.y = 180.0f;

			// カメラ動け
			cameraObject->transform.pos = crashCameraPos;
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			cameraObject->transform.pos.y = MyEase::Lerp(player->GetWorldPos().y + 20.0f,
				player->GetWorldPos().y + 10.0f,
				startPhaseTimer / startPhaseTime);
			cameraObject->transform.pos.z = MyEase::Lerp(player->GetWorldPos().z + 40.0f,
				player->GetWorldPos().z + 45.0f,
				startPhaseTimer / startPhaseTime);

			cameraObject->transform.rot.x = MyEase::Lerp(20.0f,
				10.0f,
				startPhaseTimer / startPhaseTime);
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// カメラワーク二段階(自機右上から至近距離)
	else if (startPhase == 1)
	{
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0)
		{
			//自機とカメラの距離
			const KMyMath::Vector3 playerDistance = { 2.5f, 2.5f, 3.5f };

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			cameraObject->transform.rot.x = 45.0f;
			cameraObject->transform.rot.y = 250.0f;

			// カメラ動け
			cameraObject->transform.pos = crashCameraPos;
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			cameraObject->transform.pos.z = MyEase::Lerp(player->GetWorldPos().z + 3.5f,
				player->GetWorldPos().z + 1.5f,
				startPhaseTimer / startPhaseTime);
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// カメラワーク三段階(自機左したからブースター(ケツ)注視)
	else if (startPhase == 2)
	{
		startPhaseTime = 180.0f;

		if (startPhaseTimer == 0)
		{
			//自機とカメラの距離
			const KMyMath::Vector3 playerDistance = { -2.0f, -1.8f, -3.5f };

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			cameraObject->transform.rot.x = -35.0f;
			cameraObject->transform.rot.y = 27.5f;

			// カメラ動け
			cameraObject->transform.pos = crashCameraPos;
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			cameraObject->transform.pos.x = MyEase::Lerp(player->GetWorldPos().x - 2.0f,
				player->GetWorldPos().x - 1.5f,
				startPhaseTimer / startPhaseTime);

			cameraObject->transform.rot.y = MyEase::Lerp(35.0f,
				27.5f,
				startPhaseTimer / startPhaseTime);;
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	// カメラワーク四段階(正面でカメラを引く)
	else if (startPhase == 3)
	{
		startPhaseTime = 120.0f;

		if (startPhaseTimer == 0)
		{
			//自機とカメラの距離
			const KMyMath::Vector3 playerDistance = { 0.0f, 1.0f, 10.0f };

			// カメラの場所
			const KMyMath::Vector3 crashCameraPos = player->GetWorldPos() + playerDistance;

			// 角度
			cameraObject->transform.rot.x = 0;
			cameraObject->transform.rot.y = 180.0f;

			// カメラ動け
			cameraObject->transform.pos = crashCameraPos;
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			cameraObject->transform.pos.z = MyEase::OutCubicFloat(player->GetWorldPos().z + 10.0f,
				player->GetWorldPos().z + 30.0f,
				startPhaseTimer / startPhaseTime);
		}
		else
		{
			startPhase++;
			startPhaseTimer = 0;
		}
	}
	else
	{

	}
}

void RailCamera::SetRot()
{
	// 回転
	const KMyMath::Vector3 PlayerRotDivNum = { 5,5,8 };
	cameraObject->transform.rot.x = player->GetRot().x / PlayerRotDivNum.x;
	cameraObject->transform.rot.y = player->GetRot().y / PlayerRotDivNum.y;
	cameraObject->transform.rot.z = -player->GetRot().y / PlayerRotDivNum.z;
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

void RailCamera::CallStart()
{
	// スタート状態にする
	isStart = true;
}
