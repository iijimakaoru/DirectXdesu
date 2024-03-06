#include "Camera.h"

void Camera::Init() {
	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
}

void Camera::Update() { viewProjection->Update(); }

ViewProjection* Camera::GetViewPro() { return viewProjection.get(); }

const Transform& Camera::GetTransform() const { return cameraTransform; }

const KMyMath::Vector3& Camera::GetCameraPos() const { return cameraTransform.GetPos(); }

const KMyMath::Vector3& Camera::GetCameraRot() const { return cameraTransform.GetRot(); }

const KMyMath::Vector3 Camera::GetWorldPos() const {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = cameraTransform.GetMatWorld().m[3][0];
	result.y = cameraTransform.GetMatWorld().m[3][1];
	result.z = cameraTransform.GetMatWorld().m[3][2];

	return result;
}

void Camera::SetCameraPos(const KMyMath::Vector3& pos_) { cameraTransform.SetPos(pos_); }

void Camera::SetCameraRot(const KMyMath::Vector3& rot_) { cameraTransform.SetRot(rot_); }
