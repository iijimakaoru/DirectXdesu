#include "GameCamera.h"

void GameCamera::Init()
{
	Camera::Init();

	cameraTransform.SetPos({ 0.0f, 90.0f, -130.0f });
	cameraTransform.SetRot({ 20.0f, 0.0f, 0.0f });

	cameraTransform.TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraTransform.GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	Camera::Update();
}

void GameCamera::Update()
{
	cameraTransform.TransUpdate();

	KMyMath::Matrix4 nowMatWorld = cameraTransform.GetMatWorld();
	viewProjection->SetMatView(MyMathUtility::MakeInverse(nowMatWorld));

	Camera::Update();
}
