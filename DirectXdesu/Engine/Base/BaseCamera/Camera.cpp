#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;

void Camera::Init() {
	KMyMath::Vector3 eye = viewProjection->GetEye();
	KMyMath::Vector3 target = viewProjection->GetTarget();
	KMyMath::Vector3 up = viewProjection->GetUp();

	matView = MyMathUtility::MakeLockAt(eye, target, up);
	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
}

void Camera::Update() {
	//ViewUpdate();
	viewProjection->Update();
}

void Camera::ViewUpdate() {
	KMyMath::Vector3 eyePosition = viewProjection->GetEye() * cameraTransform.GetMatWorld();
	// 注視点座標
	KMyMath::Vector3 targetPosition = viewProjection->GetTarget() * cameraTransform.GetMatWorld();
	// （仮の）上方向
	KMyMath::Vector3 upVector = viewProjection->GetUp();

	// カメラZ軸（視線方向）
	KMyMath::Vector3 cameraAxisZ = targetPosition - eyePosition;

	// 0ベクトルだと向きが定まらないので除外

	// ベクトルを正規化
	MyMathUtility::MakeNormalize(cameraAxisZ);

	// カメラのX軸（右方向）
	KMyMath::Vector3 cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = MyMathUtility::Vector3Cross(upVector, cameraAxisZ);
	// ベクトルを正規化
	MyMathUtility::MakeNormalize(cameraAxisX);

	// カメラのY軸（上方向）
	KMyMath::Vector3 cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = MyMathUtility::Vector3Cross(cameraAxisZ, cameraAxisX);

	// ここまでで直交した3方向のベクトルが揃う
	// （ワールド座標系でのカメラの右方向、上方向、前方向）

	// カメラ回転行列
	KMyMath::Matrix4 matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.m[0][0] = cameraAxisX.x;
	matCameraRot.m[0][1] = cameraAxisX.y;
	matCameraRot.m[0][2] = cameraAxisX.z;
	matCameraRot.m[0][3] = 0;
	matCameraRot.m[1][0] = cameraAxisY.x;
	matCameraRot.m[1][1] = cameraAxisY.y;
	matCameraRot.m[1][2] = cameraAxisY.z;
	matCameraRot.m[1][3] = 0;
	matCameraRot.m[2][0] = cameraAxisZ.x;
	matCameraRot.m[2][1] = cameraAxisZ.y;
	matCameraRot.m[2][2] = cameraAxisZ.z;
	matCameraRot.m[2][3] = 0;
	matCameraRot.m[3][0] = 0;
	matCameraRot.m[3][1] = 0;
	matCameraRot.m[3][2] = 0;
	matCameraRot.m[3][3] = 1;

	// 転置により逆行列（逆回転）を計算
	matView = MyMathConvert::ChangeXMMATRIXtoMatrix4(
	    XMMatrixTranspose(MyMathConvert::ChangeMatrix4toXMMATRIX(matCameraRot)));

	// 視点座標に-1を掛けた座標
	KMyMath::Vector3 reverseEyePosition = -eyePosition;
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	float tX = MyMathUtility::Vector3Dot(cameraAxisX, reverseEyePosition); // X成分
	float tY = MyMathUtility::Vector3Dot(cameraAxisY, reverseEyePosition); // Y成分
	float tZ = MyMathUtility::Vector3Dot(cameraAxisZ, reverseEyePosition); // Z成分
	// 一つのベクトルにまとめる
	KMyMath::Vector3 translation = {tX, tY, tZ};
	// ビュー行列に平行移動成分を設定
	matView.m[3][0] = translation.x;
	matView.m[3][1] = translation.y;
	matView.m[3][2] = translation.z;
	matView.m[3][3] = 1;
}

ViewProjection* Camera::GetViewPro() { return viewProjection.get(); }

const KMyMath::Matrix4& Camera::GetMatView() const { return matView; }

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
