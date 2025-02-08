#pragma once
#include<IntrinsicParameterCalibrator.h>
#include<ExtrinsiParameterCalibrator.h>

class CameraCalibrator
{
public:

	CameraCalibrator();
	~CameraCalibrator() = default;

public:

	struct Callback
	{
		virtual bool Capture() = 0;
	};

public:
	void IntrinsicParameterCalibration(void* pCap,bool imShow = false);
	bool  IntrinsicParameterSave(const std::string& outPutFilePath);
	bool  IntrinsicParameterSave(const std::string& outPutFilePath,const IntrinsicParameterCalibrator::Parameter& parameter);
	const IntrinsicParameterCalibrator::Parameter& GetIntrinsicParameter() const;
	void IntrinsicParameterCalibrationEnd();
	IntrinsicParameterCalibrator::Parameter LoadIntrinsicParameter(const std::string& filePath);

	void ExtrinsiParameterCalibration(void* pCap,const IntrinsicParameterCalibrator::Parameter& parameter,Callback* callback,bool imShow = false);
	bool ExtrinsiParameterSave(const std::string& outPutFilePath);
	bool  ExtrinsiParameterSave(const std::string& outPutFilePath,const ExtrinsiParameterCalibrator::Parameter& parameter);
	const ExtrinsiParameterCalibrator::Parameter& GetExtrinsiParameter() const;
	ExtrinsiParameterCalibrator::Parameter LoadExtrinsiParameter(const std::string& filePath);
	void ExtrinsiParameterCalibrationEnd();


private:

	const int32_t m_maxFrameCount = 10;
	int32_t m_frameCount = 0;
	std::unique_ptr<IntrinsicParameterCalibrator>m_intrinsicParameterCalibrator;
	IntrinsicParameterCalibrator::Parameter m_intrinsicParameter;
	bool m_intrinsicParameterCalibration = false;

	std::unique_ptr<ExtrinsiParameterCalibrator>m_extrinsiParameterCalibrator;
	ExtrinsiParameterCalibrator::Parameter m_extrinsiParameter;
	bool m_extrinsiParameterCalibration = false;

};