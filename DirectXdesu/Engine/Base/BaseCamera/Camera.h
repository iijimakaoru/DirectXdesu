#pragma once
#include "ViewProjection.h"
#include "KWinApp.h"
#include <memory>
#include "KObject3d.h"

class Camera
{
public:
	std::unique_ptr<ViewProjection> viewProjection = nullptr;

	std::unique_ptr<KObject3d> cameraObject = nullptr;

public:
	virtual ~Camera() {}

	virtual void Init() = 0;

	virtual void Update() = 0;

	const WorldTransfom& GetTransform()const
	{
		return cameraObject->transform;
	}

	const KMyMath::Vector3& GetPos()const
	{
		return cameraObject->transform.pos;
	}
};