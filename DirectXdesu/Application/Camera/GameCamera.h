#pragma once
#include "Camera.h"
#include <DirectXMath.h>
#include "Vector2.h"
#include "MyMath.h"

class GameCamera : public Camera
{
public:
	GameCamera() 
	{
		Init();
	};

	void Init()override;

	void Update()override;
};

