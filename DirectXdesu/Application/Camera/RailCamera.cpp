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

	isStart = true;

	startPhase = 0;

	startPhaseTimer = 0;

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
	cameraObject->AddSetRot({ 0.0f,-135.0f ,0.0f });

	// カメラ動け
	cameraObject->SetPos(crashCameraPos);
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
			cameraObject->SetRot({ 20.0f,180.0f,cameraObject->GetRot().z });

			// カメラ動け
			cameraObject->SetPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			cameraObject->SetPos(
				{ cameraObject->GetPos().x,
				MyEase::Lerp(player->GetWorldPos().y + 20.0f,player->GetWorldPos().y + 10.0f,startPhaseTimer / startPhaseTime),
				MyEase::Lerp(player->GetWorldPos().z + 40.0f,player->GetWorldPos().z + 45.0f,startPhaseTimer / startPhaseTime) }
			);

			cameraObject->SetRot(
				{ MyEase::Lerp(20.0f,10.0f,startPhaseTimer / startPhaseTime) ,
				cameraObject->GetRot().y,
				cameraObject->GetRot().z
				}
			);
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
			cameraObject->SetRot({ 45.0f,250.0f,cameraObject->GetRot().z });

			// カメラ動け
			cameraObject->SetPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			cameraObject->SetPos(
				{ cameraObject->GetPos().x,
				cameraObject->GetPos().y,
				MyEase::Lerp(player->GetWorldPos().z + 3.5f,player->GetWorldPos().z + 1.5f,startPhaseTimer / startPhaseTime) }
			);
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
			cameraObject->SetRot({ -35.0f,27.5f,cameraObject->GetRot().z });

			// カメラ動け
			cameraObject->SetPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			cameraObject->SetPos(
				{ MyEase::Lerp(player->GetWorldPos().x - 2.0f,player->GetWorldPos().x - 1.5f,startPhaseTimer / startPhaseTime),
				cameraObject->GetPos().y,
				cameraObject->GetPos().z }
			);

			cameraObject->SetRot(
				{ cameraObject->GetRot().x,
				MyEase::Lerp(35.0f,27.5f,startPhaseTimer / startPhaseTime),
				cameraObject->GetRot().z }
			);
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
			cameraObject->SetRot({ 0.0f,180.0f,cameraObject->GetRot().z });

			// カメラ動け
			cameraObject->SetPos(crashCameraPos);
		}

		if (startPhaseTimer < startPhaseTime)
		{
			startPhaseTimer++;

			cameraObject->SetPos(
				{ cameraObject->GetPos().x,
				cameraObject->GetPos().y,
				MyEase::OutCubicFloat(player->GetWorldPos().z + 10.0f,player->GetWorldPos().z + 30.0f,startPhaseTimer / startPhaseTime) }
			);
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

void RailCamera::CallStart()
{
	// スタート状態にする
	isStart = true;
}

void RailCamera::EndStart()
{
	cameraObject->SetPos(startPos);
	cameraObject->SetRot({ 0,0,0 });
}

void RailCamera::SetIsStart(bool isStart_)
{
	isStart = isStart_;
}
