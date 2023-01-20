#pragma once
#include "ViewProjection.h"
#include "KWinApp.h"
#include <memory>

class Camera
{
public:
	ViewProjection viewProjection;

	Camera(){}

	virtual ~Camera(){}

	virtual void Update(){}

	static Camera* nowCamera;
};

