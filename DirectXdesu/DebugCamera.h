#pragma once
#include "Camera.h"
#include <DirectXMath.h>
#include "Vector2.h"
#include "MyMath.h"

class DebugCamera : public Camera
{
private:
	DirectX::XMFLOAT2 angle = { 90,0 };
public:
	float moveSpeed = 0.2f;

	DebugCamera(DirectX::XMFLOAT3 pos, float moveSpeed = 0.2f) : moveSpeed(moveSpeed)
	{
		viewProjection.eye = pos;
		viewProjection.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
	}

	void Update() override;
};

