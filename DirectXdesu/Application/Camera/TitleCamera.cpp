#include "TitleCamera.h"

void TitleCamera::Init()
{
	Camera::Init();

	cameraObject->transform.pos = { 0,0,-40 };
	cameraObject->transform.rot = { 0,0,0 };

	cameraObject->TransUpdate();

	viewProjection->matView = MyMathUtility::MakeInverse(cameraObject->transform.matWorld);

	Camera::Update();
}

void TitleCamera::Update()
{
	cameraObject->TransUpdate();

	viewProjection->matView = MyMathUtility::MakeInverse(cameraObject->transform.matWorld);

	Camera::Update();
}
