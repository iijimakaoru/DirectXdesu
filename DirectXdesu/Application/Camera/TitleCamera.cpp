#include "TitleCamera.h"
#include "KInput.h"

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
	if (KInput::GetInstance()->IsPush(DIK_RIGHT))
	{
		eye.x += 1.0f;
	}
	else if (KInput::GetInstance()->IsPush(DIK_LEFT))
	{
		eye.x -= 1.0f;
	}

	if (KInput::GetInstance()->IsPush(DIK_UP))
	{
		eye.z += 1.0f;
	}
	else if (KInput::GetInstance()->IsPush(DIK_DOWN))
	{
		eye.z -= 1.0f;
	}

	viewProjection->SetMatView(MyMathUtility::MakeLockAt(eye, target, up));

	Camera::Update();
}
