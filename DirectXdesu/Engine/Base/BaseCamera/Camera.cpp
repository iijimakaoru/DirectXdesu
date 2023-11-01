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
	return cameraObject->GetTransform();
}

const KMyMath::Vector3& Camera::GetCameraPos() const
{
	return cameraObject->GetPos();
}

const KMyMath::Vector3& Camera::GetCameraRot() const
{
	return cameraObject->GetRot();
}

const KMyMath::Vector3 Camera::GetWorldPos() const
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = cameraObject->GetMatWorld().m[3][0];
	result.y = cameraObject->GetMatWorld().m[3][1];
	result.z = cameraObject->GetMatWorld().m[3][2];

	return result;
}

void Camera::SetCameraPos(const KMyMath::Vector3& pos_)
{
	cameraObject->SetPos(pos_);
}

void Camera::SetCameraRot(const KMyMath::Vector3& rot_)
{
	cameraObject->SetRot(rot_);
}
