#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "MyMath.h"

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