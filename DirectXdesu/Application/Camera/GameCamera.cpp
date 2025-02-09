 #include "GameCamera.h"

void GameCamera::Init()
{
	Camera::Init();

	cameraTransform.SetPos({ 0.0f, 70.0f, -175.0f });
	cameraTransform.SetRot({ 10.0f, 0.0f, 0.0f });

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
