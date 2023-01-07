#include "DebugCamera.h"
#include "KInput.h"

//void DebugCamera::Update()
//{
//	if (KInput::GetInstance()->IsPush(DIK_D))
//	{
//		viewProjection.eye.x += moveSpeed;
//	}
//
//	if (KInput::GetInstance()->IsPush(DIK_A))
//	{
//		viewProjection.eye.x += -moveSpeed;
//	}
//
//	if (KInput::GetInstance()->IsPush(DIK_W))
//	{
//		viewProjection.eye.z += moveSpeed;
//	}
//
//	if (KInput::GetInstance()->IsPush(DIK_S))
//	{
//		viewProjection.eye.z += -moveSpeed;
//	}
//	// デバッグカメラ高さ調整
//	if (KInput::GetInstance()->IsPush(DIK_Q))
//	{
//		viewProjection.eye.y += moveSpeed;
//	}
//	if (KInput::GetInstance()->IsPush(DIK_E))
//	{
//		viewProjection.eye.y -= moveSpeed;
//	}
//
//	if (angle.x >= 360)
//	{
//		angle.x -= 360;
//	}
//	if (angle.x < 0)
//	{
//		angle.x += 360;
//	}
//
//	if (angle.y >= 90)
//	{
//		angle.y = 89.9f;
//	}
//	if (angle.y <= -90)
//	{		  
//		angle.y = -89.9f;
//	}
//
//	viewProjection.target.x = viewProjection.eye.x - 100 * cosf(PI / 180 * angle.x) * cosf(PI / 180 * angle.y);
//	viewProjection.target.y = viewProjection.eye.y + 100 * sinf(PI / 180 * angle.y);
//	viewProjection.target.z = viewProjection.eye.z + 100 * sinf(PI / 180 * angle.x) * cosf(PI / 180 * angle.y);
//
//	viewProjection.Update();
//}
