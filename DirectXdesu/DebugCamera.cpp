#include "DebugCamera.h"
#include "KInput.h"

DebugCamera::DebugCamera()
{
	viewProjection.Initialize();
	viewProjection.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
}

void DebugCamera::Update()
{
	/// <summary>
	/// 場所変更
	/// </summary>
	if (KInput::GetInstance()->IsPush(DIK_D))
	{
		viewProjection.eye.x += 0.5f;
	}

	if (KInput::GetInstance()->IsPush(DIK_A))
	{
		viewProjection.eye.x += -0.5f;
	}

	if (KInput::GetInstance()->IsPush(DIK_W))
	{
		viewProjection.eye.z += 0.5f;
	}

	if (KInput::GetInstance()->IsPush(DIK_S))
	{
		viewProjection.eye.z += -0.5f;
	}
	// デバッグカメラ高さ調整
	if (KInput::GetInstance()->IsPush(DIK_Q))
	{
		viewProjection.eye.y += 0.5f;
	}
	if (KInput::GetInstance()->IsPush(DIK_E))
	{
		viewProjection.eye.y -= 0.5f;
	}

	/// <summary>
	/// 角度変更
	/// </summary>
	if (KInput::GetInstance()->IsPush(DIK_J))
	{
		viewProjection.angleX -= XMConvertToRadians(20.0f);
	}
	if (KInput::GetInstance()->IsPush(DIK_L))
	{
		viewProjection.angleX += XMConvertToRadians(20.0f);
	}
	if (KInput::GetInstance()->IsPush(DIK_I))
	{
		viewProjection.angleY += XMConvertToRadians(20.0f);
	}
	if (KInput::GetInstance()->IsPush(DIK_K))
	{
		viewProjection.angleY -= XMConvertToRadians(20.0f);
	}

	if (viewProjection.angleX >= 360)
	{
		viewProjection.angleX -= 360;
	}
	if (viewProjection.angleX < 0)
	{
		viewProjection.angleX += 360;
	}

	if (viewProjection.angleY >= 90)
	{
		viewProjection.angleY = 89.9f;
	}
	if (viewProjection.angleY <= -90)
	{
		viewProjection.angleY = -89.9f;
	}

	/*viewProjection.target.x = viewProjection.eye.x - 100 * cosf(PI / 180 * viewProjection.angleX) * cosf(PI / 180 * viewProjection.angleY);
	viewProjection.target.y = viewProjection.eye.y + 100 * sinf(PI / 180 * viewProjection.angleY);
	viewProjection.target.z = viewProjection.eye.z + 100 * sinf(PI / 180 * viewProjection.angleX) * cosf(PI / 180 * viewProjection.angleY);*/

	viewProjection.target = { 0,20,0 };

	viewProjection.Update();
}