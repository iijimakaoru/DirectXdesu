#pragma once

#include "Capture.h"
#include <array>
#include <memory>
#include <iostream>
#include <vector>
#include <cmath>

namespace MCBM
{
	enum Locate : int32_t
	{
		FRONT,
		BACK,
		RIGHT,
		LEFT
	};
	class CaptureManager
	{

	private:
		std::array<Capture, 4> capdatas;
		std::unordered_map<YOLO_POSE_INDEX, CaptureData> finalCaptureData_;
	public:
		void Initialize();
		void Update();
		void CalclateFinalCaptureData();
		void InitializePose();
		CaptureData& GetCaptureData(YOLO_POSE_INDEX key);

	};
}
