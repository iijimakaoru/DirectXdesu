#pragma once
#include <opencv2/opencv.hpp>
#include<utility>
#include<string>

#include<CaptureMatrix3.h>
#include<CaptureVector4.h>

class IntrinsicParameterCalibrator
{
public:
	IntrinsicParameterCalibrator() = default;
	virtual ~IntrinsicParameterCalibrator() = default;

public:

	struct Parameter
	{
		CaptureMatrix3 cameraMatrix;
		CaptureVector5 distortionCoefficients;
		double repError;
	};

public:

	static IntrinsicParameterCalibrator* Create();

	virtual void Reset() = 0;
	virtual int32_t GetMaxCapturedImage() = 0;
	virtual std::pair<bool,int32_t> Capture(cv::Mat& frame,bool capture) = 0;
	virtual std::pair<bool,Parameter> Calibrate() = 0;
	virtual bool Save(const std::string& outPutFilePath) = 0;
};
