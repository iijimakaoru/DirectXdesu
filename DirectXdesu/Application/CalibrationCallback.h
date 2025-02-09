#pragma once
#include "CameraCalibrator.h"
class CalibrationCallback :public CameraCalibrator::Callback
{
	bool callBack = false;

public:
	bool Capture() override;
	void SetCapture(bool flag);
};

