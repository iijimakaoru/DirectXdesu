 #include "GameCamera.h"

void GameCamera::Init()
{
	Camera::Init();

	cameraTransform.SetPos({ 0.0f, 50.0f, -155.0f });
	cameraTransform.SetRot({ 0.0f, 0.0f, 0.0f });

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
