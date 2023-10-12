#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "MyMath.h"

/**
 * @file DebugCamera.h
 * @brief デバッグカメラ
 * @author 飯島 薫
 */

class DebugCamera : public Camera
{
public:
	DebugCamera() 
	{
		Init();
	};

	void Init() override;

	void Update() override;
};