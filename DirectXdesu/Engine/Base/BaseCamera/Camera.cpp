#include "Camera.h"

void Camera::Init()
{
	cameraObject = std::make_unique<KObject3d>();

	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
}

void Camera::Update()
{
	viewProjection->Update();
}

ViewProjection* Camera::GetViewPro()
{
	return viewProjection.get();
}

const WorldTransfom& Camera::GetTransform() const
{
	return cameraObject->transform;
}

const KMyMath::Vector3& Camera::GetPos() const
{
	return cameraObject->transform.pos;
}

void Camera::SetPos(const KMyMath::Vector3 pos)
{
	cameraObject->transform.pos = pos;
}

void Camera::SetWorldPosXY(const float x, const float y)
{
	cameraObject->transform.matWorld.m[3][0] = x;
	cameraObject->transform.matWorld.m[3][1] = y;
}

void Camera::SetPosXY(const float x, const float y)
{
	cameraObject->transform.pos.x = x;
	cameraObject->transform.pos.y = y;
}

void Camera::SetRotZ(const float rotZ)
{
	cameraObject->transform.rot.z = rotZ;
}

const KMyMath::Vector3 Camera::GetWorldPos() const
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = cameraObject->transform.matWorld.m[3][0];
	result.y = cameraObject->transform.matWorld.m[3][1];
	result.z = cameraObject->transform.matWorld.m[3][2];

	return result;
}
