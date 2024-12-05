#include "TitleCamera.h"
#include "KInput.h"

#include "Ease.h"

void TitleCamera::Init() {
	Camera::Init();

	cameraTransform.SetPos({0.0f, 0.0f, -20.0f});
	cameraTransform.SetRot({0.0f, 0.0f, 0.0f});

	cameraTransform.TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraTransform.GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	isRound = false;

	isSortie = false;

	sortiePhase = 0;

	sortiePhaseTimer = 0;

	sortiePhaseTime = 0;

	Camera::Update();
}

void TitleCamera::Update() {
	if (isRound) {
		RoundCamera();
	} else if (isSortie) {
		SortieCamera();
	}

	cameraTransform.TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraTransform.GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	Camera::Update();
}

void TitleCamera::RoundCamera() {
	cameraTransform.SetPos({cameraTransform.GetPos().x, 4.0f, cameraTransform.GetPos().z});

	// 角度を変更
	const float rotSpeed = 0.5f;
	rotAngle += rotSpeed;

	// 360を超えたら
	if (rotAngle >= 360) {
		rotAngle = 0;
	}

	nowAngle = rotAngle;

	const float radian = DirectX::XMConvertToRadians(rotAngle);
	const float distance = -20;
	cameraTransform.SetPos(
	    {distance * sinf(radian), cameraTransform.GetPos().y, distance * cosf(radian)});

	cameraTransform.SetRot({2.5f, rotAngle, cameraTransform.GetRot().z});
}

void TitleCamera::StartRound() { isRound = true; }

void TitleCamera::SortieCamera() {
	if (sortiePhase == 0) {
		sortiePhaseTime = 45;

		if (sortiePhaseTimer < sortiePhaseTime) {
			sortiePhaseTimer++;
		} else {
			sortiePhase++;
		}

		cameraTransform.SetPos(
		    {cameraTransform.GetPos().x,
		     MyEase::OutCubicFloat(3, 0, sortiePhaseTimer / sortiePhaseTime),
		     cameraTransform.GetPos().z});

		float radian = MyEase::OutCubicFloat(
		    DirectX::XMConvertToRadians(nowAngle), DirectX::XMConvertToRadians(360 * 2),
		    sortiePhaseTimer / sortiePhaseTime);
		float distance = MyEase::OutCubicFloat(-40, -30, sortiePhaseTimer / sortiePhaseTime);

		cameraTransform.SetPos(
		    {distance * sinf(radian), cameraTransform.GetPos().y, distance * cosf(radian)});

		cameraTransform.SetRot(
		    {MyEase::OutCubicFloat(2, 0, sortiePhaseTimer / sortiePhaseTime),
		     DirectX::XMConvertToDegrees(radian), cameraTransform.GetRot().z});
	} else {
		isSortie = false;
	}
}

void TitleCamera::StartSortie() {
	isRound = false;
	isSortie = true;
}
