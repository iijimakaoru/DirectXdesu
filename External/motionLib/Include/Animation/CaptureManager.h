#pragma once

#include <Animation/Capture.h>
#include <array>
#include <memory>
#include <iostream>
#include <vector>
#include <cmath>
#include <YOLOPoseEstimation.h>

namespace MCBM
{

	class CaptureManager
	{

	private:
		std::array<Capture,4> capdatas;
		std::unordered_map<YOLO_POSE_INDEX,CaptureData> finalCaptureData_;
		std::unique_ptr<YOLOPoseEstimation> m_YOLOPoseEstimation_;
		const std::string& modelPath_ = "Resources/Checkpoints/yolo11x-pose.onnx";
		const int32_t cameraFirstIndex = 0;
	public:
		void Initialize();
		void Update();
		void InitializePose();
		void Finalize();
		CaptureData& GetCaptureData(YOLO_POSE_INDEX key);
		CaptureData& GetLocateCaptureData(YOLO_POSE_INDEX key,Locate locate);

	};
}
