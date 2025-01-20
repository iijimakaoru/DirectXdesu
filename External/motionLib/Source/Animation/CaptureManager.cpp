#include "CaptureManager.h"
#include <Math/Matrix3x3.h>

void MCBM::CaptureManager::Initialize()
{
	for (int32_t i = 0; i < 4; i++)
	{
		capdatas[i].Initialize(static_cast<int32_t>(i));
	}

	capdatas[Locate::FRONT].cameraPosition_ = { 0.0f,0.0f,capdatas[Locate::FRONT].cameradist_ };
	capdatas[Locate::BACK].cameraPosition_ = { 0.0f,0.0f,-capdatas[Locate::BACK].cameradist_ };
	capdatas[Locate::LEFT].cameraPosition_ = { -capdatas[Locate::LEFT].cameradist_,0.0f,0.0f };
	capdatas[Locate::RIGHT].cameraPosition_ = { capdatas[Locate::RIGHT].cameradist_,0.0f,0.0f };
}

void MCBM::CaptureManager::Update()
{
	for (size_t i = 0; i < 4; i++)
	{
		capdatas[i].Update();
	}
	CalclateFinalCaptureData();
}

void MCBM::CaptureManager::CalclateFinalCaptureData()
{
	Vector3 finalData;
	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		CaptureData frontCamera = capdatas[Locate::FRONT].GetCaptureData((YOLO_POSE_INDEX)i);
		CaptureData backCamera = capdatas[Locate::BACK].GetCaptureData((YOLO_POSE_INDEX)i);
		CaptureData leftCamera = capdatas[Locate::LEFT].GetCaptureData((YOLO_POSE_INDEX)i);
		CaptureData rightCamera = capdatas[Locate::RIGHT].GetCaptureData((YOLO_POSE_INDEX)i);

		Vector3 tmpF(frontCamera.captureBonePos.x - capdatas[Locate::FRONT].screenCenterPos_.x
			, frontCamera.captureBonePos.y - capdatas[Locate::FRONT].screenCenterPos_.y,
			-capdatas[Locate::FRONT].focalLength_);

		Vector3 tmpB(backCamera.captureBonePos.x - capdatas[Locate::BACK].screenCenterPos_.x,
			backCamera.captureBonePos.y - capdatas[Locate::BACK].screenCenterPos_.y,
			capdatas[Locate::BACK].focalLength_);

		Vector3 tmpL(capdatas[Locate::LEFT].focalLength_,
			leftCamera.captureBonePos.y - capdatas[Locate::LEFT].screenCenterPos_.y,
			-(leftCamera.captureBonePos.x - capdatas[Locate::LEFT].screenCenterPos_.x));

		Vector3 tmpR(-capdatas[Locate::RIGHT].focalLength_,
			rightCamera.captureBonePos.y - capdatas[Locate::RIGHT].screenCenterPos_.y,
			+(rightCamera.captureBonePos.x - capdatas[Locate::RIGHT].screenCenterPos_.x));

		Vector3 dF = tmpF.GetV3Norm();
		Vector3 dB = tmpB.GetV3Norm();
		Vector3 dL = tmpL.GetV3Norm();
		Vector3 dR = tmpR.GetV3Norm();

		Matrix3x3 Q;
		for (int i = 0; i < 9; i++)
		{
			Q.mat[i] = 0.0;
		}
		Vector3 C(0.0, 0.0, 0.0);

		auto accumulate_line = [&](const Vector3& p, const Vector3& d, double w)
			{
				// P = I - d d^T
				Matrix3x3 P = P.ProjectionMatrix(d);
				// Q += w * P
				Matrix3x3 wP = wP.Mat3Scale(P, w);
				Q = Q.Mat3Add(Q, wP);
				// c += w * P * p
				Vector3 Pp = P.Mat3Mulvec(P, p);
				C = C + (w * Pp);
			};

		// 前カメラ
		accumulate_line(capdatas[Locate::FRONT].cameraPosition_, dF, frontCamera.captureBonePos.z);
		// 後カメラ
		accumulate_line(capdatas[Locate::BACK].cameraPosition_, dB, backCamera.captureBonePos.z);
		// 左カメラ
		accumulate_line(capdatas[Locate::LEFT].cameraPosition_, dL, leftCamera.captureBonePos.z);
		// 右カメラ
		accumulate_line(capdatas[Locate::RIGHT].cameraPosition_, dR, rightCamera.captureBonePos.z);

		// (6) 連立方程式 Q X = C を解く (Xが最小二乗解)
		Matrix3x3 Qinv;
		bool ok = Q.Invert3x3(Q, Qinv);
		Vector3 X(0, 0, 0);
		if (ok)
		{
			X = Q.Mat3Mulvec(Qinv, C);
		}
		else
		{
			// Qが特異 → 全部平行などの場合。
			// ここでは簡単に(0,0,0)を返す
			std::cerr << "警告: 行列が特異です。解けませんでした。\n";
		}
		finalData = X;
		finalCaptureData_[(YOLO_POSE_INDEX)i].captureBonePos = finalData;
	}
}

void MCBM::CaptureManager::InitializePose()
{
	for (int32_t i = 0; i < 4; i++)
	{
		capdatas[i].Update();
	}
	CalclateFinalCaptureData();

	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		finalCaptureData_[(YOLO_POSE_INDEX)i].initializedCaptureBonePos
			= finalCaptureData_[(YOLO_POSE_INDEX)i].captureBonePos;
	}


}

MCBM::CaptureData& MCBM::CaptureManager::GetCaptureData(YOLO_POSE_INDEX key)
{
	return finalCaptureData_[key];
}

