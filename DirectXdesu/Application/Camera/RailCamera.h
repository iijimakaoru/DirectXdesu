#pragma once
#include "Camera.h"
#include "KObject3d.h"

class RailCamera : public Camera
{
public:
	RailCamera() {};

	void Init() override;

	void Update()override;

	void SetPos(KMyMath::Vector3 pos);

	void SetWorldTransform(WorldTransfom transform_)
	{
		object->transform = transform_;
	}

	void SetRadian(float angle_)
	{
		angle = angle_;
	}

private:
	std::unique_ptr<KObject3d> object;

	float angle = 0;
};