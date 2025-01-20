#include "TitleCamera.h"
#include "KInput.h"

#include "Ease.h"

void TitleCamera::Init() {
	Camera::Init();

	cameraTransform.SetPos({0.0f, 0.0f, -10.0f});
	cameraTransform.SetRot({0.0f, 0.0f, 0.0f});

	cameraTransform.TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraTransform.GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	isRound = true;

	isSortie = false;

	sortiePhase = 0;

	sortiePhaseTimer = 0;

	sortiePhaseTime = 0;

	Camera::Update();
}

void TitleCamera::Update() {
	cameraTransform.TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraTransform.GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	Camera::Update();
}
