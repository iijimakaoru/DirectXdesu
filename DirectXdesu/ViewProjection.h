#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Vector3.h"

using namespace DirectX;

class ViewProjection
{
public:
	ViewProjection();
	void Initialize(int width, int height);
	void Update(int width, int height);

	// カメラの距離
	float lenZ = 0;
	float angleX = 0; // カメラの回転角X
	float angleY = 0; // カメラの回転角Y
	// ビュー変換行列
	XMMATRIX matView{};
	XMFLOAT3 eye{};
	XMFLOAT3 target{};
	XMFLOAT3 up{};
	// 射影変換行列の計算
	XMMATRIX matProjection{};
};

