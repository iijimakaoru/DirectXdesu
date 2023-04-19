#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Vector3.h"
#include "KWinApp.h"

using namespace DirectX;

class ViewProjection
{
public:
	ViewProjection();
	void Initialize();
	void Update();

	// JÌ£
	float lenZ = 0;
	float angleX = 0; // JÌñ]pX
	float angleY = 0; // JÌñ]pY
	// r[Ï·sñ
	XMMATRIX matView{};
	XMFLOAT3 eye{};
	XMFLOAT3 target{};
	XMFLOAT3 up{};
	// ËeÏ·sñÌvZ
	XMMATRIX matProjection{};

	float aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH(); // AXyNgä
};

