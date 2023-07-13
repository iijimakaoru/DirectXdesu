#pragma once
#include "ViewProjection.h"
#include "KWinApp.h"
#include <memory>
#include "KObject3d.h"

class Camera
{
protected:
	std::unique_ptr<ViewProjection> viewProjection = nullptr;

	std::unique_ptr<KObject3d> cameraObject = nullptr;

public:
	virtual ~Camera() {}

	virtual void Init();

	virtual void Update();

	ViewProjection* GetViewPro();

	const WorldTransfom& GetTransform()const;

	const KMyMath::Vector3& GetPos()const;

	void SetPos(const KMyMath::Vector3 pos);

	void SetWorldPosXY(const float x, const float y);

	void SetPosXY(const float x, const float y);

	void SetRotZ(const float rotZ);

	const KMyMath::Vector3 GetWorldPos() const;
};