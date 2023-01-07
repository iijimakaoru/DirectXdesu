#pragma once
#include "ViewProjection.h"
#include "KWinApp.h"
#include <memory>

class Camera
{
public:
	ViewProjection viewProjection;

	float PI = 3.1415927f;

	Camera()
	{
		viewProjection.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
	}
	virtual ~Camera(){}

	virtual void Update(){}

	static Camera* nowCamera;
};

