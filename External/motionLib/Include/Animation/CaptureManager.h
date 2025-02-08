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
		std::array<Capture, Locate::MAX_LOCATE> capdatas;
		std::unordered_map<YOLO_POSE_INDEX, CaptureData> finalCaptureData_;
		std::unique_ptr<YOLOPoseEstimation> m_YOLOPoseEstimation_;
		const std::string& modelPath_ = "Resources/Checkpoints/yolo11x-pose.onnx";
		const int32_t cameraFirstIndex = 0;

		std::array<std::string, 17> linkBoneNames = { "none","none" ,"none" ,"none" ,"none" ,
														"upper_arm.L","upper_arm.R",
														"forearm.L","forearm.R","hand.L",
													"hand.R" ,"none" ,"none" ,"none" ,"none" ,
													"none","none" };

		int32_t cameraNum_;

		CaptureManager();
		CaptureManager(const CaptureManager& captureManager) = delete;
		CaptureManager& operator=(const CaptureManager& captureManager) = delete;
		~CaptureManager();

		bool initialized = false;

	public:
		static CaptureManager* GetInstance();
		void Initialize(int32_t cameraNum = Locate::MAX_LOCATE);
		void Update();
		void InitializePose();
		void Finalize();
		CaptureData& GetCaptureData(YOLO_POSE_INDEX key);
		CaptureData& GetLocateCaptureData(YOLO_POSE_INDEX key,Locate locate);
		YOLOPoseEstimation* GetYOLOPoseEstimation();

	};
}
