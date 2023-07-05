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

	// カメラの距離
	float lenZ = 0;
	float angleX = 0; // カメラの回転角X
	float angleY = 0; // カメラの回転角Y
	// ビュー変換行列
	KMyMath::Matrix4 matView{};
	KMyMath::Vector3 eye{};
	KMyMath::Vector3 target{};
	KMyMath::Vector3 up{};
	// 射影変換行列の計算
	KMyMath::Matrix4 matProjection{};

	float aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH(); // アスペクト比
};

