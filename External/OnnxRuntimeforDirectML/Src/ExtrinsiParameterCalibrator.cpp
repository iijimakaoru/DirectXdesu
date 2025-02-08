#include "ExtrinsiParameterCalibrator.h"

#include <opencv2/opencv.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <json.hpp>

#include<vector>
#include<fstream>

class ExtrinsiParameterCalibratorImp final : public ExtrinsiParameterCalibrator
{
public:
	ExtrinsiParameterCalibratorImp();
	~ExtrinsiParameterCalibratorImp();

public:

	void Reset() override;
	std::pair<bool,ExtrinsiParameterCalibrator::Parameter> Capture(cv::Mat& frame,const IntrinsicParameterCalibrator::Parameter& parameter,bool capture) override;
	bool Save(const std::string& outPutFilePPath)override;

private:

	// マーカーのサイズ (メートル単位)
	float m_markerLength = 0.101;  // 5cm = 0.05m

	int  m_dictionaryId = cv::aruco::DICT_5X5_250;
	cv::aruco::ArucoDetector m_arucoDetector;
	cv::aruco::Dictionary m_dictionary;
	cv::aruco::DetectorParameters m_detectorParams;

	Parameter m_parameter;
	IntrinsicParameterCalibrator::Parameter m_intrinsicParameter;

	bool m_capture = false;

	cv::Mat m_cameraMatrix;
	cv::Mat m_distCoeffs;
	std::vector<cv::Vec3d> m_rvecs,m_tvecs;
};

ExtrinsiParameterCalibratorImp::ExtrinsiParameterCalibratorImp()
{
	m_dictionary = cv::aruco::getPredefinedDictionary(m_dictionaryId);
	m_detectorParams = cv::aruco::DetectorParameters();
	m_arucoDetector = cv::aruco::ArucoDetector(m_dictionary,m_detectorParams);
}

ExtrinsiParameterCalibratorImp::~ExtrinsiParameterCalibratorImp()
{
}

void ExtrinsiParameterCalibratorImp::Reset()
{
	m_capture = false;
}

std::pair<bool,ExtrinsiParameterCalibrator::Parameter> ExtrinsiParameterCalibratorImp::Capture(cv::Mat& frame,const IntrinsicParameterCalibrator::Parameter& parameter,bool capture)
{
	if ( m_capture )
	{
		return std::pair<bool,ExtrinsiParameterCalibrator::Parameter>(false,ExtrinsiParameterCalibrator::Parameter());
	}

	m_cameraMatrix = ( cv::Mat_<double>(3,3) <<
		parameter.cameraMatrix.Get(0,0),parameter.cameraMatrix.Get(1,0),parameter.cameraMatrix.Get(2,0),
		parameter.cameraMatrix.Get(0,1),parameter.cameraMatrix.Get(1,1),parameter.cameraMatrix.Get(2,1),
		parameter.cameraMatrix.Get(0,2),parameter.cameraMatrix.Get(1,2),parameter.cameraMatrix.Get(2,2) );
	m_distCoeffs = ( cv::Mat_<double>(1,5) << parameter.distortionCoefficients.GetX(),parameter.distortionCoefficients.GetY(),parameter.distortionCoefficients.GetZ(),parameter.distortionCoefficients.GetW(),parameter.distortionCoefficients.GetV() );

	cv::Mat copyImage;
	frame.copyTo(copyImage);
	cv::Mat gray;

	cv::cvtColor(frame,gray,cv::COLOR_RGB2GRAY);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners,rejected;

	// マーカーを検出
	// detect markers and estimate pose
	m_arucoDetector.detectMarkers(frame,corners,ids,rejected);

	if ( ids.size() > 0 )
	{
		// マーカーが検出された場合、ポーズを推定
		cv::aruco::drawDetectedMarkers(frame,corners,ids);
		cv::aruco::estimatePoseSingleMarkers(corners,m_markerLength,m_cameraMatrix,m_distCoeffs,m_rvecs,m_tvecs);

		// 検出されたマーカーそれぞれに対して座標軸を描画
		for ( size_t i = 0; i < m_rvecs.size(); i++ )
		{
			cv::drawFrameAxes(frame,m_cameraMatrix,m_distCoeffs,m_rvecs[ i ],m_tvecs[ i ],m_markerLength * 0.5f);
		}

		if ( capture )
		{
			cv::Mat R;
			cv::Rodrigues(m_rvecs[ 0 ],R);

			for ( int32_t i = 0; i < R.rows; i++ )
			{
				for ( int32_t j = 0; j < R.cols; j++ )
				{
					m_parameter.rotationMatrix.Set(i,j,R.at<double>(j,i));
				}
			}

			for ( int32_t i = 0; i < 3; i++ )
			{
				m_parameter.rotationVector.Set(i,m_rvecs[ 0 ][ i ]);
			}

			for ( int32_t i = 0; i < 3; i++ )
			{
				m_parameter.translationVector.Set(i,m_tvecs[ 0 ][ i ]);
			}

			m_capture = true;

			return std::pair<bool,ExtrinsiParameterCalibrator::Parameter>(true,m_parameter);

		}
	}

	return std::pair<bool,ExtrinsiParameterCalibrator::Parameter>(false,ExtrinsiParameterCalibrator::Parameter());
}

bool ExtrinsiParameterCalibratorImp::Save(const std::string& outPutFilePPath)
{
	if ( m_capture )
	{
		// 現在の日時を取得
		auto t = time(nullptr);
		auto tm_ = tm();
		char buf[ 256 ] = { 0 };
		localtime_s(&tm_,&t);
		strftime(buf,256,"%Y/%m/%d %H:%M:%S%z\n",&tm_);
		std::string datetime = buf;
		datetime.pop_back(); // 改行文字を削除

		cv::FileStorage fs(outPutFilePPath,cv::FileStorage::WRITE | cv::FileStorage::FORMAT_JSON);
		if ( !fs.isOpened() )
		{
			return false;
		}

		cv::Mat R;
		cv::Rodrigues(m_rvecs[ 0 ],R);

		fs << "rotation_matrix" << R;
		fs << "rotation_vector" << m_rvecs[ 0 ];
		fs << "translation_vector" << m_tvecs[ 0 ];
		fs << "calibration_date" << datetime;

		return true;
	}
	else
	{
		return false;
	}
}

ExtrinsiParameterCalibrator* ExtrinsiParameterCalibrator::Create()
{
	return new ExtrinsiParameterCalibratorImp();
}
