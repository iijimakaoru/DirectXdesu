#pragma once
#include "Camera.h"
#include <DirectXMath.h>
#include "Vector2.h"
#include "MyMath.h"

class DebugCamera : public Camera
{
public:
	DebugCamera();

	void Update() override;
};