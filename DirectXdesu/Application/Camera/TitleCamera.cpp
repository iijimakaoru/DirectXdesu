#include "TitleCamera.h"
#include "KInput.h"

void TitleCamera::Init()
{
	Camera::Init();

	cameraObject->transform.pos = { 0,0,-40 };
	cameraObject->transform.rot = { 0,0,0 };

	cameraObject->TransUpdate();

	viewProjection->SetMatView(MyMathUtility::MakeInverse(cameraObject->transform.matWorld));

	isRound = false;

	Camera::Update();
}

void TitleCamera::Update()
{
	if (isRound)
	{
		RoundCamera();
	}

	cameraObject->TransUpdate();

	viewProjection->SetMatView(MyMathUtility::MakeInverse(cameraObject->transform.matWorld));

	Camera::Update();
}

void TitleCamera::RoundCamera()
{
	// Šp“x‚ð•ÏX
	const float rotSpeed = 0.5f;
	rotAngle += rotSpeed;

	// 360‚ð’´‚¦‚½‚ç
	if (rotAngle >= 360)
	{
		rotAngle = 0;
	}

	const float radian = XMConvertToRadians(rotAngle);
	const float distance = -40;
	cameraObject->transform.pos.z = distance * cosf(radian);
	cameraObject->transform.pos.x = distance * sinf(radian);

	cameraObject->transform.rot.y = rotAngle;
}

void TitleCamera::StartRound()
{
	isRound = true;
}
