#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Vector3.h"

using namespace DirectX;

class ViewProjection
{
public:
	ViewProjection();
	ViewProjection(int width, int height);
	void Initialize(int width, int height);
	void Update(int width, int height);

	// JÌ£
	float lenZ;
	float angleX; // JÌñ]pX
	float angleY; // JÌñ]pY
	// r[Ï·sñ
	XMMATRIX matView;
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;
	// ËeÏ·sñÌvZ
	XMMATRIX matProjection;
};

