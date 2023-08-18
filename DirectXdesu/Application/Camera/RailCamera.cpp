#include "RailCamera.h"
#include "MyMath.h"
#include "Player.h"

const float RailCamera::moveSpeedPlayerMagnification = 8.0f;

const float RailCamera::advanceSpeed = 1.0f;

void RailCamera::Init()
{
	cameraObject = std::make_unique<KObject3d>();
	cameraObject->transform.pos = { 0,0,-200 };
	cameraObject->transform.scale = { 1.0f,1.0f,1.0f };
	cameraObject->transform.rot = { 0,0,0 };

	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
	viewProjection->aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
}

void RailCamera::Update(Player* player)
{
	moveLimitMax = Player::GetPosLimitMax();
	moveLimitMin = Player::GetPosLimitMin();

	Move();

	if (!player->GetIsDead())
	{
		SetRot(player->GetRot());
	}

	cameraObject->TransUpdate();

	viewProjection->matView = MyMathUtility::MakeInverse(cameraObject->transform.matWorld);

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

	float moveSpeedPhaseSpeed = 0.5f;

	if (isAdvance)
	{
		velocity.z = advanceSpeed * moveSpeedPhaseSpeed;
	}

	// 移動
	cameraObject->transform.pos += velocity;

	// 移動限界から動くな
	cameraObject->transform.pos.x = max(cameraObject->transform.pos.x, moveLimitMin.x);
	cameraObject->transform.pos.x = min(cameraObject->transform.pos.x, moveLimitMax.x);
	cameraObject->transform.pos.y = max(cameraObject->transform.pos.y, moveLimitMin.y);
	cameraObject->transform.pos.y = min(cameraObject->transform.pos.y, moveLimitMax.y);
}

void RailCamera::SetRot(const KMyMath::Vector3& playersRot)
{
	// 回転
	const KMyMath::Vector3 PlayerRotDivNum = { 5,5,8 };
	cameraObject->transform.rot.x = playersRot.x / PlayerRotDivNum.x;
	cameraObject->transform.rot.y = playersRot.y / PlayerRotDivNum.y;
	cameraObject->transform.rot.z = -playersRot.y / PlayerRotDivNum.z;
}

const float RailCamera::GetSpeed() const
{
	return advanceSpeed * 0.5f;
}
