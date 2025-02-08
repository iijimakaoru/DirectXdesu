#include "CameraCalibrator.h"

#include <opencv2/opencv.hpp>
#include<json.hpp>
#include<fstream>

CameraCalibrator::CameraCalibrator()
{
	m_intrinsicParameterCalibrator.reset(IntrinsicParameterCalibrator::Create());

	m_extrinsiParameterCalibrator.reset(ExtrinsiParameterCalibrator::Create());
}

void CameraCalibrator::IntrinsicParameterCalibration(void* pCap,bool imShow)
{
	m_intrinsicParameterCalibration = true;

	m_intrinsicParameterCalibrator->Reset();

	cv::Mat frame;

	cv::VideoCapture* pCapture = ( cv::VideoCapture* ) pCap;

	while ( true )
	{
		pCapture->read(frame);

		auto&& result = m_intrinsicParameterCalibrator->Capture(frame,m_frameCount == 0);

		if ( result.first )
		{
			if ( result.second == m_intrinsicParameterCalibrator->GetMaxCapturedImage() )
			{
				break;
			}
		}

		if ( m_frameCount == 0 )
		{
			m_frameCount = m_maxFrameCount;
		}
		else
		{
			m_frameCount--;
		}

		if ( imShow )
		{
			cv::imshow("IntrinsicParameterCalibration",frame);
			cv::waitKey(1);
		}
	}

	auto&& result = m_intrinsicParameterCalibrator->Calibrate();

	m_intrinsicParameter = result.second;

	if ( imShow )
	{
		cv::destroyWindow("IntrinsicParameterCalibration");
	}

	IntrinsicParameterCalibrationEnd();
}

bool CameraCalibrator::IntrinsicParameterSave(const std::string& outPutFilePath)
{
	m_intrinsicParameterCalibration = false;

	return m_intrinsicParameterCalibrator->Save(outPutFilePath + ".json");
}

bool CameraCalibrator::IntrinsicParameterSave(const std::string& outPutFilePath, const IntrinsicParameterCalibrator::Parameter& parameter)
{
	m_intrinsicParameterCalibration = false;

	return m_intrinsicParameterCalibrator->Save(outPutFilePath + ".json", parameter);
}

const IntrinsicParameterCalibrator::Parameter& CameraCalibrator::GetIntrinsicParameter() const
{
	return m_intrinsicParameter;
}

void CameraCalibrator::IntrinsicParameterCalibrationEnd()
{
	m_intrinsicParameterCalibration = false;
}

IntrinsicParameterCalibrator::Parameter CameraCalibrator::LoadIntrinsicParameter(const std::string& filePath)
{
	IntrinsicParameterCalibrator::Parameter parameter;

	std::ifstream lFile;

	lFile.open(filePath);

	if ( lFile.fail() )
	{
		assert(0);
	}

	nlohmann::json deserialized;
	lFile >> deserialized;
	assert(deserialized.is_object());

	float buf[ 10 ] = {0};

	for ( size_t i = 0; i< 9; i++ )
	{
		buf[i]=static_cast< float>(deserialized[ "camera_matrix" ][ "data" ][i] );
	}

	parameter.cameraMatrix.SetValues(buf);

	for ( size_t i = 0; i < 5; i++ )
	{
		buf[ i ] = static_cast< float >( deserialized[ "distortion_coefficients" ][ "data" ][ i ] );
	}

	parameter.distortionCoefficients.SetValues(buf);

	parameter.repError = static_cast< float >( deserialized[ "reprojection_error" ] );


	return parameter;
}

void CameraCalibrator::ExtrinsiParameterCalibration(void* pCap,const IntrinsicParameterCalibrator::Parameter& parameter,Callback* callback,bool imShow)
{
	m_extrinsiParameterCalibration = true;

	m_extrinsiParameterCalibrator->Reset();

	cv::Mat frame;

	cv::VideoCapture* pCapture = ( cv::VideoCapture* ) pCap;

	while ( true )
	{
		pCapture->read(frame);

		auto&& result = m_extrinsiParameterCalibrator->Capture(frame,parameter,callback->Capture());

		if ( result.first )
		{
			m_extrinsiParameter = result.second;

			break;
		}

		if ( imShow )
		{
			cv::imshow("ExtrinsiParameterCalibration",frame);
			cv::waitKey(1);
		}
	}

	if ( imShow )
	{
		cv::destroyWindow("ExtrinsiParameterCalibration");
	}

	ExtrinsiParameterCalibrationEnd();
}

bool CameraCalibrator::ExtrinsiParameterSave(const std::string& outPutFilePath)
{
	m_extrinsiParameterCalibration = false;

	return m_extrinsiParameterCalibrator->Save(outPutFilePath + ".json");
}

bool CameraCalibrator::ExtrinsiParameterSave(const std::string& outPutFilePath, const ExtrinsiParameterCalibrator::Parameter& parameter)
{
	m_extrinsiParameterCalibration = false;

	return m_extrinsiParameterCalibrator->Save(outPutFilePath + ".json", parameter);
}

const ExtrinsiParameterCalibrator::Parameter& CameraCalibrator::GetExtrinsiParameter() const
{
	return m_extrinsiParameter;
}

ExtrinsiParameterCalibrator::Parameter CameraCalibrator::LoadExtrinsiParameter(const std::string& filePath)
{
	ExtrinsiParameterCalibrator::Parameter parameter;

	std::ifstream file;

	file.open(filePath);

	if ( file.fail() )
	{
		assert(0);
	}

	nlohmann::json deserialized;
	file >> deserialized;
	assert(deserialized.is_object());

	float buf[ 10 ] = { 0 };

	for ( size_t i = 0; i < 9; i++ )
	{
		buf[ i ] = static_cast< float >( deserialized[ "rotation_matrix" ][ "data" ][ i ] );
	}

	parameter.rotationMatrix.SetValues(buf);

	for ( size_t i = 0; i < 3; i++ )
	{
		buf[ i ] = static_cast< float >( deserialized[ "rotation_vector" ][ i ] );
	}

	parameter.rotationVector.SetValues(buf);

	for ( size_t i = 0; i < 3; i++ )
	{
		buf[ i ] = static_cast< float >( deserialized[ "translation_vector" ][ i ] );
	}

	parameter.translationVector.SetValues(buf);

	return parameter;
}

void CameraCalibrator::ExtrinsiParameterCalibrationEnd()
{
	m_extrinsiParameterCalibration = false;
}
