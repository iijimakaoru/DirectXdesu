#pragma once
#include<IntrinsicParameterCalibrator.h>
#include<CaptureVector3.h>
class ExtrinsiParameterCalibrator
{
public:
	ExtrinsiParameterCalibrator() = default;
	virtual ~ExtrinsiParameterCalibrator() = default;

public:

	struct Parameter
	{
		CaptureMatrix3 rotationMatrix;
		CaptureVector3 rotationVector;
		CaptureVector3 translationVector;
	};

public:

	static ExtrinsiParameterCalibrator* Create();

	virtual void Reset() = 0;
	virtual std::pair<bool,ExtrinsiParameterCalibrator::Parameter> Capture(cv::Mat& frame,const IntrinsicParameterCalibrator::Parameter& intrinsicParameter,bool capture) = 0;
	virtual bool Save(const std::string& outPutFilePath) = 0;
};