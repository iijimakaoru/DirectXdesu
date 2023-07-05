#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Vector3.h"
#include "KWinApp.h"
#include "MyMath.h"

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
	KMyMath::Matrix4 matView{};
	KMyMath::Vector3 eye{};
	KMyMath::Vector3 target{};
	KMyMath::Vector3 up{};
	// ËeÏ·sñÌvZ
	KMyMath::Matrix4 matProjection{};

	float aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH(); // AXyNgä
};

