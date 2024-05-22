#include "ViewProjection.h"
#include "KWinApp.h"

ViewProjection::ViewProjection() {}

void ViewProjection::Initialize() {
	eye = {0, 0, 0};
	target = {0, 0, 0};
	up = {0, 1, 0};
	nearZ = 0.1f;
	farZ = 1000.0f;

	aspect = static_cast<float>(KWinApp::GetWindowSizeW()) /
	         static_cast<float>(KWinApp::GetWindowSizeH());

	matProjection =
	    MyMathUtility::MakePerspective(DirectX::XMConvertToRadians(45.0f), aspect, nearZ, farZ);
}

void ViewProjection::Update() {
	matProjection =
	    MyMathUtility::MakePerspective(DirectX::XMConvertToRadians(45.0f), aspect, nearZ, farZ);
}

void ViewProjection::ViewUpdate() { // 視点座標
}

const KMyMath::Matrix4& ViewProjection::GetMatPro() const { return matProjection; }

const KMyMath::Vector3& ViewProjection::GetEye() const { return eye; }

const KMyMath::Vector3& ViewProjection::GetTarget() const { return target; }

const KMyMath::Vector3& ViewProjection::GetUp() const { return up; }

void ViewProjection::SetEye(const KMyMath::Vector3& eye_) { eye = eye_; }

void ViewProjection::SetTarget(const KMyMath::Vector3& target_) { target = target_; }

void ViewProjection::SetUp(const KMyMath::Vector3& up_) { up = up_; }

void ViewProjection::SetFarZ(const float& farZ_) { farZ = farZ_; }

void ViewProjection::SetNearZ(const float& nearZ_) { nearZ = nearZ_; }
