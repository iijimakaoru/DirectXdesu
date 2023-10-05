#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "MyMath.h"

class GameCamera : public Camera
{
public:
	GameCamera() 
	{
		Init();
	};

	void Update()override;
};

