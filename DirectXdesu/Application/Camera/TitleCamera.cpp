#include "TitleCamera.h"
#include "KInput.h"

#include "Ease.h"

void TitleCamera::Init()
{
	Camera::Init();

	cameraObject->transform.pos = { 0,0,-40 };
	cameraObject->transform.rot = { 0,0,0 };

	cameraObject->TransUpdate();

	viewProjection->SetMatView(MyMathUtility::MakeInverse(cameraObject->transform.matWorld));

	isRound = false;

	isSortie = false;

	sortiePhase = 0;

	sortiePhaseTimer = 0;

	sortiePhaseTime = 0;

	Camera::Update();
}

void TitleCamera::Update()
{
	if (isRound)
	{
		RoundCamera();
	}
	else if (isSortie)
	{
		SortieCamera();
	}

	cameraObject->TransUpdate();

	viewProjection->SetMatView(MyMathUtility::MakeInverse(cameraObject->transform.matWorld));

	Camera::Update();
}

void TitleCamera::RoundCamera()
{
	cameraObject->transform.pos.y = 3;

	// Šp“x‚ð•ÏX
	const float rotSpeed = 0.5f;
	rotAngle += rotSpeed;

	// 360‚ð’´‚¦‚½‚ç
	if (rotAngle >= 360)
	{
		rotAngle = 0;
	}

	nowAngle = rotAngle;

	const float radian = XMConvertToRadians(rotAngle);
	const float distance = -40;
	cameraObject->transform.pos.z = distance * cosf(radian);
	cameraObject->transform.pos.x = distance * sinf(radian);

	cameraObject->transform.rot.y = rotAngle;
	cameraObject->transform.rot.x = 2.5f;
}

void TitleCamera::StartRound()
{
	isRound = true;
}

void TitleCamera::SortieCamera()
{
	if (sortiePhase == 0)
	{
		sortiePhaseTime = 45;

		if (sortiePhaseTimer < sortiePhaseTime)
		{
			sortiePhaseTimer++;
		}
		else
		{
			sortiePhase++;
		}

		cameraObject->transform.pos.y = MyEase::OutCubicFloat(3, 0, sortiePhaseTimer / sortiePhaseTime);

		float radian = 
			MyEase::OutCubicFloat(XMConvertToRadians(nowAngle),
				XMConvertToRadians(360 * 2),
				sortiePhaseTimer / sortiePhaseTime);
		float distance = MyEase::OutCubicFloat(-40, -30, sortiePhaseTimer / sortiePhaseTime);

		cameraObject->transform.pos.z = distance * cosf(radian);
		cameraObject->transform.pos.x = distance * sinf(radian);

		cameraObject->transform.rot.y = XMConvertToDegrees(radian);
		cameraObject->transform.rot.x = MyEase::OutCubicFloat(2, 0, sortiePhaseTimer / sortiePhaseTime);
	}
	else
	{
		isSortie = false;
	}
}

void TitleCamera::StartSortie()
{
	isRound = false;
	isSortie = true;
}
