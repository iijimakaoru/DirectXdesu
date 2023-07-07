#pragma once
#include "Camera.h"
#include "KObject3d.h"

class RailCamera : public Camera
{
public:
	RailCamera() 
	{
		Init();
	};

	void Init() override;

	void Update()override;

	void SetRadian(float angle_)
	{
		angle = angle_;
	}

private:
	float angle = 0;
};