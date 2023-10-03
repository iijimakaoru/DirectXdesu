#include "TitleCamera.h"

void TitleCamera::Init()
{
	Camera::Init();

	viewProjection->SetEye({ 0,0,-40 });
	viewProjection->SetTarget({ 0,0,0 });

	eye = viewProjection->GetEye();
	target = viewProjection->GetTarget();
	up = viewProjection->GetUp();

	Camera::Update();
}

void TitleCamera::Update()
{
	viewProjection->SetMatView(MyMathUtility::MakeLockAt(eye, target, up));

	Camera::Update();
}
