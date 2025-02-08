#include <IntrinsicParameterCalibrator.h>

#include <opencv2/opencv.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <json.hpp>

#include<vector>
#include<fstream>

class IntrinsicParameterCalibratorImp final : public IntrinsicParameterCalibrator
{
public:
	IntrinsicParameterCalibratorImp();
	~IntrinsicParameterCalibratorImp();

public:

	void Reset() override;
	std::pair<bool,int32_t> Capture(cv::Mat& frame,bool capture) override;
	std::pair<bool,Parameter> Calibrate() override;
	bool Save(const std::string& outPutFilePPath)override;
	int32_t GetMaxCapturedImage()override;

private:

	const cv::Size m_squaresSize = { 16,9 };	//!< ボードサイズ
	const float m_squareLength = 0.018f;			//!< メートル単位
	const float m_markerLength = 0.013f;			//!< メートル単位
	const int32_t m_maxCapturedImage = 20;
private:


	cv::Ptr<cv::aruco::CharucoBoard> m_charucoBoard;
	cv::Ptr<cv::aruco::DetectorParameters> m_detectorParams;
	cv::Ptr<cv::aruco::Dictionary> m_dictionary;

	std::vector<cv::Mat> m_allImgs;
	std::vector<std::vector<cv::Point2f>> m_allCharucoCorners;
	std::vector<std::vector<int>> m_allCharucoIds;
	cv::Size m_imageSize;

	Parameter m_parameter;

	int32_t m_capturedImages;
	bool m_calibrate = false;
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

};

IntrinsicParameterCalibratorImp::IntrinsicParameterCalibratorImp()
{
	// ArUcoマーカー辞書の設定
	m_dictionary = cv::makePtr<cv::aruco::Dictionary>(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250));
	// ChArUcoボードの設定
	m_charucoBoard = cv::makePtr<cv::aruco::CharucoBoard>(m_squaresSize,m_squareLength,m_markerLength,*m_dictionary);
	// 検出パラメータの設定
	m_detectorParams = cv::makePtr<cv::aruco::DetectorParameters>();
}

IntrinsicParameterCalibratorImp::~IntrinsicParameterCalibratorImp()
{
}

void IntrinsicParameterCalibratorImp::Reset()
{
	for ( size_t i = 0; i < m_allCharucoCorners.size(); i++ )
	{
		m_allCharucoCorners[ i ].clear();
	}
	m_allCharucoCorners.clear();

	for ( size_t i = 0; i < m_allCharucoCorners.size(); i++ )
	{
		m_allCharucoIds[ i ].clear();
	}
	m_allCharucoIds.clear();

	m_allImgs.clear();
	m_capturedImages = 0;
	m_calibrate = false;
}

std::pair<bool,int32_t> IntrinsicParameterCalibratorImp::Capture(cv::Mat& frame,bool capture)
{
	if ( frame.empty() )
	{
		return std::pair<bool,int32_t>(false,-1);
	}

	if ( m_capturedImages == 0 )
	{
		m_imageSize = frame.size();
	}

	if ( m_imageSize != frame.size() )
	{
		return std::pair<bool,int32_t>(false,-1);
	}

	// ArUcoマーカーの検出
	std::vector<std::vector<cv::Point2f>> markerCorners,rejectedCandidates;
	std::vector<int> markerIds;
	cv::aruco::detectMarkers(frame,m_dictionary,markerCorners,markerIds,m_detectorParams,rejectedCandidates);

	// マーカーが検出された場合
	if ( !markerIds.empty() )
	{
		// 検出されたマーカーの描画
		cv::aruco::drawDetectedMarkers(frame,markerCorners,markerIds);

		// ChArUcoコーナーの補間
		std::vector<cv::Point2f> charucoCorners;
		std::vector<int> charucoIds;
		cv::aruco::interpolateCornersCharuco(markerCorners,markerIds,frame,m_charucoBoard,charucoCorners,charucoIds);

		// ChArUcoコーナーが検出された場合
		if ( charucoIds.size() > 0 )
		{
			cv::aruco::drawDetectedCornersCharuco(frame,charucoCorners,charucoIds,cv::Scalar(255,0,0));

			if ( capture && charucoCorners.size() > 4 )
			{
				m_allCharucoCorners.push_back(charucoCorners);
				m_allCharucoIds.push_back(charucoIds);
				m_allImgs.push_back(frame);
				m_capturedImages++;

				std::cout << "画像 " << m_maxCapturedImage << "/" << m_capturedImages << " を取得しました" << std::endl;


				return std::pair<bool,int32_t>(true,m_capturedImages);
			}
		}
	}

	return std::pair<bool,int32_t>(false,-1);
}

std::pair<bool,IntrinsicParameterCalibrator::Parameter> IntrinsicParameterCalibratorImp::Calibrate()
{
	if ( m_capturedImages < m_maxCapturedImage )
	{
		return std::pair<bool,IntrinsicParameterCalibrator::Parameter>(true,IntrinsicParameterCalibrator::Parameter());
	}

	// キャリブレーションの実行
	std::vector<cv::Mat> rvecs,tvecs;

	m_parameter.repError = cv::aruco::calibrateCameraCharuco(m_allCharucoCorners,m_allCharucoIds,m_charucoBoard,m_imageSize,cameraMatrix,distCoeffs,rvecs,tvecs);

	for ( int32_t i = 0; i < cameraMatrix.rows; i++ )
	{
		for ( int32_t j = 0; j < cameraMatrix.cols; j++ )
		{
			m_parameter.cameraMatrix.Set(i,j,cameraMatrix.at<double>(j,i));
		}
	}

	for ( int32_t i = 0; i < distCoeffs.total(); i++ )
	{
		m_parameter.distortionCoefficients.Set(i,distCoeffs.at<double>(i));
	}

	m_calibrate = true;

	return std::pair<bool,IntrinsicParameterCalibrator::Parameter>(true,m_parameter);
}

bool IntrinsicParameterCalibratorImp::Save(const std::string& outPutFilePPath)
{
	if ( m_capturedImages < m_maxCapturedImage && m_calibrate )
	{
		return false;
	}

	cv::FileStorage fs(outPutFilePPath,cv::FileStorage::WRITE| cv::FileStorage::FORMAT_JSON);
	if ( !fs.isOpened() )
	{
		return false;
	}

	auto t = time(nullptr);
	auto tm_ = tm();
	char buf[ 256 ] = { 0 };
	localtime_s(&tm_,&t);
	strftime(buf,256,"%Y/%m/%d %H:%M:%S%z\n",&tm_);
	std::string datetime = buf;
	datetime.pop_back(); // 改行文字を削除

	fs << "camera_matrix" << cameraMatrix;
	fs << "distortion_coefficients" << distCoeffs;
	fs << "reprojection_error" << m_parameter.repError;
	fs << "calibration_time" << datetime;

	return true;
}

int32_t IntrinsicParameterCalibratorImp::GetMaxCapturedImage()
{
	return m_maxCapturedImage;
}

IntrinsicParameterCalibrator* IntrinsicParameterCalibrator::Create()
{
	return new IntrinsicParameterCalibratorImp();
}
