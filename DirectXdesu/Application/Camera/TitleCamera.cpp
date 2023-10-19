#include "TitleCamera.h"
#include "KInput.h"

#include "Ease.h"

void TitleCamera::Init()
{
	Camera::Init();

	cameraObject->SetPos({ 0.0f,0.0f,-20.0f });
	cameraObject->SetRot({ 0.0f,0.0f,0.0f });

	cameraObject->TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraObject->GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

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

	KMyMath::Matrix4 nowMatWorld = cameraObject->GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	Camera::Update();
}

void TitleCamera::RoundCamera()
{
	cameraObject->SetPos({ cameraObject->GetPos().x, 4.0f,cameraObject->GetPos().z });

	// 角度を変更
	const float rotSpeed = 0.5f;
	rotAngle += rotSpeed;

	// 360を超えたら
	if (rotAngle >= 360)
	{
		rotAngle = 0;
	}

	nowAngle = rotAngle;

	const float radian = DirectX::XMConvertToRadians(rotAngle);
	const float distance = -20;
	cameraObject->SetPos({ distance * sinf(radian) ,cameraObject->GetPos().y ,distance * cosf(radian) });

	cameraObject->SetRot({ 2.5f ,rotAngle ,cameraObject->GetRot().z });
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

		cameraObject->SetPos({ cameraObject->GetPos().x,
			MyEase::OutCubicFloat(3, 0, sortiePhaseTimer / sortiePhaseTime) ,
			cameraObject->GetPos().z });

		float radian = 
			MyEase::OutCubicFloat(DirectX::XMConvertToRadians(nowAngle),
				DirectX::XMConvertToRadians(360 * 2),
				sortiePhaseTimer / sortiePhaseTime);
		float distance = MyEase::OutCubicFloat(-40, -30, sortiePhaseTimer / sortiePhaseTime);

		cameraObject->SetPos({ distance * sinf(radian) ,cameraObject->GetPos().y ,distance * cosf(radian) });

		cameraObject->SetRot({ MyEase::OutCubicFloat(2, 0, sortiePhaseTimer / sortiePhaseTime) ,
			DirectX::XMConvertToDegrees(radian),
			cameraObject->GetRot().z });
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
