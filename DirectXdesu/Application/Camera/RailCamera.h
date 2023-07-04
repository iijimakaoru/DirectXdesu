#pragma once
#include "Camera.h"
#include "KObject3d.h"

class RailCamera : public Camera
{
	RailCamera() {};

	void Init() override;

	void Update()override;

private:
	WorldTransfom transform;
};