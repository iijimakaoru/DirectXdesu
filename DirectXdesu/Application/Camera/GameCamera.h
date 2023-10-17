#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "MyMath.h"

/**
 * @file GameCamera.h
 * @brief ゲームカメラ
 * @author 飯島 薫
 */

class GameCamera : public Camera
{
public:
	GameCamera() 
	{
		Init();
	};

	// 更新
	void Update()override;
};

