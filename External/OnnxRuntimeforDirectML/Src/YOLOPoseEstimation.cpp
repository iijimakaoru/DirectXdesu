#include "YOLOPoseEstimation.h"
#include <SimpleVector3.h>
#include <SimpleMatrix3x3.h>

#include<filesystem>
#include<array>
#include<vector>
#include<string>
#include<memory>
#include<thread>
#include <mutex>
#include <atomic>

#include "autobackend.h"
#include <opencv2/opencv.hpp>

#include "augment.h"
#include <dml_provider_factory.h>
#include <onnxruntime_cxx_api.h>
#include <json.hpp>

#include "constants.h"
#include "common.h"
#include "onnx_model_base.h"



using namespace MCBO;

struct Vec2
{
	int32_t x;
	int32_t y;
};

struct Vec2F
{
	float x;
	float y;
};

struct CaptureData
{
	std::string captureBoneName;
	MCBO::YVector3 captureBonePos;
	MCBO::YVector3 initializedCaptureBonePos;
	std::vector<CaptureData*> captureChildren;
	CaptureData* parent;

};


Vec2F Midpoint(const Vec2F& p1,const Vec2F& p2)
{
	Vec2F mid;
	mid.x = ( p1.x + p2.x ) / 2.0;
	mid.y = ( p1.y + p2.y ) / 2.0;
	return mid;
}

Vec2F Translate(const Vec2F& p,const Vec2F& origin)
{
	Vec2F translated_point;
	translated_point.x = p.x - origin.x;
	translated_point.y = p.y - origin.y;
	return translated_point;
}

class YOLOPoseEstimationImp : public YOLOPoseEstimation
{
public:
	YOLOPoseEstimationImp();
	~YOLOPoseEstimationImp();

public:

	void CameraInitialize(void* cam) override;

	void ModelInitialize(const char* modelPath,float mask_threshold,float conf_threshold,float iou_threshold,ONNXP_ROVIDERS provider) override;

	void Start(bool isDraw) override;
	void Initialize() override;

	const YOLO_POSE_LANDMAKE* const GetLandmakes(int32_t index) override;

	void End() override;

	void Update();

	const std::unordered_map <YOLO_POSE_INDEX,YVector3>* const GetFinalPositions() override;


	void InterinsCalibrateStart(int32_t cameraIndex) override;
	void InterinsCalibrateSave(const std::string& filepath) override;
	void ExtrinsCalibrateStart(int32_t cameraIndex) override;
	void ExtrinsCalibrateSave(const std::string& filepath) override;

	void ExtrinsCalibrateLoad(const std::string& filepath) override;
	void InterinsCalibrateLoad(const std::string& filepath) override;

	void SetCalibrateCallBack(CameraCalibrator::Callback* callBackPtr) override;

	const ExtrinsiParameterCalibrator::Parameter GetExtrinsiParameter(int32_t cameraIndex) override;
	const IntrinsicParameterCalibrator::Parameter GetInterinsParameter(int32_t cameraIndex) override;

private:

	void _Draw(cv::Mat& image,int index);

	void CalclateFinalCaptureDataFromCalibrateData();

	void AddCameraData(const std::string& filepath);

	void SetOutSideData();

private:

	const std::array<Vec2,19> m_skeleton = { {{16, 14}, {14, 12}, {17, 15}, {15, 13}, {12, 13}, {6, 12}, {7, 13}, {6, 7},{6, 8}, {7, 9}, {8, 10}, {9, 11}, {2, 3}, {1, 2}, {1, 3}, {2, 4}, {3, 5}, {4, 6}, {5, 7} } };
	const std::array<cv::Scalar,4> m_posePalette = { cv::Scalar(255, 128, 0), cv::Scalar(255, 51, 255), cv::Scalar(51, 153, 255),cv::Scalar(0, 255, 0) };
	const std::array<int32_t,19> m_limbColorIndices = { 2, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3 };
	const std::array<int32_t,17> m_kptColorIndices = { 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2 };

	const std::array<std::string,3> m_provider = { {{"cpu"},{"cuda"},{"directml"}} };

	std::vector < std::array<YOLO_POSE_LANDMAKE,17>> m_baseLandmakes;
	std::vector < std::array<YOLO_POSE_LANDMAKE,17>> m_landmakes;
	std::vector<cv::VideoCapture* >m_pCams;
	std::vector<cv::Mat> m_frame;

	float m_maskThreshold;
	float m_confhhreshold;
	float m_iouThreshold;
	int32_t m_conversionCode = cv::COLOR_BGR2RGB;

	std::unique_ptr<AutoBackendOnnx> m_pModel;
	std::string m_modelPath;
	const std::string m_onnxLogid = "yolov8_inference2";

	bool m_isDraw;
	bool m_canDraw;
	std::atomic<bool> isRunning;
	std::thread th;
	std::mutex value_mutex;


	std::array<std::unordered_map<YOLO_POSE_INDEX,CaptureData>,MAX_LOCATE> capturedata_;
	std::unordered_map <YOLO_POSE_INDEX,YVector3> finalCaptureData_;

	std::vector<ExtrinsiParameterCalibrator::Parameter> extrinsiParams;
	std::vector<IntrinsicParameterCalibrator::Parameter> instrinsiParams;

	std::unique_ptr<CameraCalibrator> calibrator;

	CameraCalibrator::Callback* callBack;

};

YOLOPoseEstimation* CreateYOLOPoseEstimation()
{
	static YOLOPoseEstimation* result;

	if ( !result )
	{
		result = new YOLOPoseEstimationImp();
	}

	return result;
}

YOLOPoseEstimationImp::YOLOPoseEstimationImp()
{
}

YOLOPoseEstimationImp::~YOLOPoseEstimationImp()
{

}

void YOLOPoseEstimationImp::CameraInitialize(void* cam)
{
	m_pCams.push_back(( cv::VideoCapture* ) cam);
	ExtrinsiParameterCalibrator::Parameter extrinsiParam;
	IntrinsicParameterCalibrator::Parameter instrinsiParam;
	instrinsiParams.push_back(instrinsiParam);
	extrinsiParams.push_back(extrinsiParam);
}

void YOLOPoseEstimationImp::ModelInitialize(const char* modelPath,float maskThreshold,float confThreshold,float iouThreshold,ONNXP_ROVIDERS provider)
{
	m_modelPath = modelPath;
	m_maskThreshold = maskThreshold;
	m_confhhreshold = confThreshold;
	m_iouThreshold = iouThreshold;

	m_pModel = std::make_unique<AutoBackendOnnx>(m_modelPath.c_str(),m_onnxLogid.c_str(),m_provider[ size_t(provider) ].c_str());
}

void YOLOPoseEstimationImp::Start(bool isDraw)
{
	m_isDraw = isDraw;
	isRunning = true;

	m_frame.resize(m_pCams.size());
	m_baseLandmakes.resize(m_pCams.size());
	m_landmakes.resize(m_pCams.size());


	for ( int32_t i = 0; i < Locate::MAX_LOCATE; i++ )
	{
		for ( int32_t j = 0; j < ( int32_t ) YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; j++ )
		{
			capturedata_[ i ][ ( YOLO_POSE_INDEX ) j ].captureBonePos = { 0,0,0 };
		}
	}

	for ( int32_t j = 0; j < ( int32_t ) YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; j++ )
	{
		finalCaptureData_[ ( YOLO_POSE_INDEX ) j ] = { 0,0,0 };
	}

	th = std::thread([ this ] ()
		{
			this->Update();
		});
}

void YOLOPoseEstimationImp::Initialize()
{
	calibrator = std::make_unique<CameraCalibrator>();
}

const YOLO_POSE_LANDMAKE* const YOLOPoseEstimationImp::GetLandmakes(int32_t index)
{
	std::lock_guard<std::mutex> lock(value_mutex);
	return m_landmakes[ index ].data();
}

void YOLOPoseEstimationImp::End()
{
	isRunning = false;

	if ( th.joinable() )
	{
		th.join();
	}
}

void YOLOPoseEstimationImp::Update()
{
	m_canDraw = false;

	while ( isRunning )
	{
		for ( size_t i = 0; i < m_pCams.size(); i++ )
		{
			m_pCams[ i ]->read(m_frame[ i ]);
		}

		for ( size_t i = 0; i < m_pCams.size(); i++ )
		{

			std::vector<YoloResults> objs;

			if ( m_pModel )
			{
				objs = m_pModel->predict_once(m_frame[ i ],m_confhhreshold,m_iouThreshold,m_maskThreshold);
			}

			if ( !objs.empty() )
			{
				for ( int j = 0; j < ( int ) YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; j++ )
				{
					int idx = j * 3;
					m_baseLandmakes[ i ][ j ].x = objs[ 0 ].keypoints[ idx ];
					m_baseLandmakes[ i ][ j ].y = objs[ 0 ].keypoints[ idx + 1 ];
					m_baseLandmakes[ i ][ j ].vi = objs[ 0 ].keypoints[ idx + 2 ];
				}

				Vec2F mid = Midpoint({ m_baseLandmakes[ i ][ size_t(YOLO_POSE_INDEX::HIP_L) ].x, m_baseLandmakes[ i ][ size_t(YOLO_POSE_INDEX::HIP_L) ].y },{ m_baseLandmakes[ i ][ size_t(YOLO_POSE_INDEX::HIP_R) ].x, m_baseLandmakes[ i ][ size_t(YOLO_POSE_INDEX::HIP_R) ].y });

				std::lock_guard<std::mutex> lock(value_mutex);

				for ( int j = 0; j < ( int ) YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; j++ )
				{
					Vec2F newPoint = Translate({ m_baseLandmakes[ i ][ j ].x, m_baseLandmakes[ i ][ j ].y },mid);

					m_landmakes[ i ][ j ].x = newPoint.x;
					m_landmakes[ i ][ j ].y = newPoint.y;
					m_landmakes[ i ][ j ].vi = m_baseLandmakes[ i ][ j ].vi;

					capturedata_[ i ][ ( YOLO_POSE_INDEX ) j ].captureBonePos.x = m_landmakes[ i ][ j ].x;
					capturedata_[ i ][ ( YOLO_POSE_INDEX ) j ].captureBonePos.y = m_landmakes[ i ][ j ].y;
					capturedata_[ i ][ ( YOLO_POSE_INDEX ) j ].captureBonePos.z = m_landmakes[ i ][ j ].vi;
				}

				m_canDraw = true;
			}

			if ( m_isDraw )
			{
				_Draw(m_frame[ i ],i);
			}

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	for ( size_t i = 0; i < m_pCams.size(); i++ )
	{
		//cv::destroyWindow("win");
		cv::destroyWindow(std::format("win{}",i));
	}
}

const std::unordered_map<YOLO_POSE_INDEX,YVector3>* const YOLOPoseEstimationImp::GetFinalPositions()
{
	return &finalCaptureData_;
}
void YOLOPoseEstimationImp::InterinsCalibrateStart(int32_t cameraIndex)
{
	calibrator->IntrinsicParameterCalibration(m_pCams[ cameraIndex ],true);
	instrinsiParams[ cameraIndex ] = calibrator->GetIntrinsicParameter();
}

void YOLOPoseEstimationImp::InterinsCalibrateSave(const std::string& filepath)
{
	for ( int32_t i = 0; i < m_pCams.size(); i++ )
	{
		std::string fullPath = filepath + "InterinsCalibrate" + std::to_string(i) + "Camera";

		calibrator->IntrinsicParameterSave(fullPath,instrinsiParams[ i ]);

	}
}

void YOLOPoseEstimationImp::ExtrinsCalibrateStart(int32_t cameraIndex)
{
	calibrator->ExtrinsiParameterCalibration(m_pCams[ cameraIndex ],instrinsiParams[ cameraIndex ],callBack,true);
	extrinsiParams[ cameraIndex ] = calibrator->GetExtrinsiParameter();
}

void YOLOPoseEstimationImp::ExtrinsCalibrateSave(const std::string& filepath)
{
	for ( int32_t i = 0; i < m_pCams.size(); i++ )
	{
		std::string fullPath = filepath + "ExtrinsCalibrate" + std::to_string(i) + "Camera";

		calibrator->ExtrinsiParameterSave(fullPath,extrinsiParams[ i ]);

	}
}

void YOLOPoseEstimationImp::ExtrinsCalibrateLoad(const std::string& filepath)
{

	for ( int32_t i = 0; i < m_pCams.size(); i++ )
	{
		std::string fullPath = filepath + "ExtrinsCalibrate" + std::to_string(i) + "Camera.json";

		extrinsiParams[ i ] = calibrator->LoadExtrinsiParameter(fullPath);

	}
}

void YOLOPoseEstimationImp::InterinsCalibrateLoad(const std::string& filepath)
{

	for ( int32_t i = 0; i < m_pCams.size(); i++ )
	{
		IntrinsicParameterCalibrator::Parameter param;
		std::string fullPath = filepath + "InterinsCalibrate" + std::to_string(i) + "Camera.json";

		param = calibrator->LoadIntrinsicParameter(fullPath);

		instrinsiParams[i] = param;

	}
}

void YOLOPoseEstimationImp::SetCalibrateCallBack(CameraCalibrator::Callback* callBackPtr)
{
	callBack = callBackPtr;
}

const ExtrinsiParameterCalibrator::Parameter YOLOPoseEstimationImp::GetExtrinsiParameter(int32_t cameraIndex)
{
	return extrinsiParams[ cameraIndex ];
}

const IntrinsicParameterCalibrator::Parameter YOLOPoseEstimationImp::GetInterinsParameter(int32_t cameraIndex)
{
	return instrinsiParams[ cameraIndex ];
}

void YOLOPoseEstimationImp::_Draw(cv::Mat& image,int index)
{
	cv::cvtColor(image,image,cv::COLOR_RGB2BGR);
	cv::Size show_shape = image.size();

	if ( m_canDraw )
	{
		for ( int i = 0; i < m_baseLandmakes[ index ].size(); i++ )
		{
			if ( m_baseLandmakes[ index ][ i ].vi < 0.5 )
			{
				continue;
			}

			cv::circle(image,cv::Point(m_baseLandmakes[ index ][ i ].x,m_baseLandmakes[ index ][ i ].y),5,m_posePalette[ m_kptColorIndices[ i ] ],-1,cv::LINE_AA);
		}

		for ( int i = 0; i < m_skeleton.size(); i++ )
		{
			const Vec2& sk = m_skeleton[ i ];

			int idx1 = sk.x - 1;
			int idx2 = sk.y - 1;

			int idx1_x_pos = idx1;
			int idx2_x_pos = idx2;

			int x1 = static_cast< int >( m_baseLandmakes[ index ][ idx1_x_pos ].x );
			int y1 = static_cast< int >( m_baseLandmakes[ index ][ idx1_x_pos ].y );
			int x2 = static_cast< int >( m_baseLandmakes[ index ][ idx2_x_pos ].x );
			int y2 = static_cast< int >( m_baseLandmakes[ index ][ idx2_x_pos ].y );

			float conf1 = m_baseLandmakes[ index ][ idx1_x_pos ].vi;
			float conf2 = m_baseLandmakes[ index ][ idx2_x_pos ].vi;

			if ( conf1 < 0.5 || conf2 < 0.5 )
			{
				continue;
			}

			if ( x1 % show_shape.width == 0 || y1 % show_shape.height == 0 || x1 < 0 || y1 < 0 ||
				x2 % show_shape.width == 0 || y2 % show_shape.height == 0 || x2 < 0 || y2 < 0 )
			{
				continue;
			}

			cv::Scalar color_limb = m_posePalette[ m_limbColorIndices[ i ] ];
			cv::line(image,cv::Point(x1,y1),cv::Point(x2,y2),color_limb,2,cv::LINE_AA);
		}
		CalclateFinalCaptureDataFromCalibrateData();
	}

	cv::imshow(std::format("win{}",index),image);
	cv::waitKey(1);
}

void YOLOPoseEstimationImp::AddCameraData(const std::string& filepath)
{

}

void YOLOPoseEstimationImp::SetOutSideData()
{

}

void YOLOPoseEstimationImp::CalclateFinalCaptureDataFromCalibrateData() {


	if ( m_pCams.size() <= 1 )
	{
		for ( size_t i = 0; i < ( int32_t ) YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++ )
		{
			finalCaptureData_[ ( YOLO_POSE_INDEX ) i ] = capturedata_[ Locate::FRONT ][ ( YOLO_POSE_INDEX ) i ].captureBonePos;
			finalCaptureData_[ ( YOLO_POSE_INDEX ) i ].z = 0;
		}
		return;
	}


	cv::Mat R1 = ( cv::Mat_<double>(3,3) <<
		extrinsiParams[ Locate::FRONT ].rotationMatrix.Get(0,0),
		extrinsiParams[ Locate::FRONT ].rotationMatrix.Get(1,0),
		extrinsiParams[ Locate::FRONT ].rotationMatrix.Get(2,0),
		extrinsiParams[ Locate::FRONT ].rotationMatrix.Get(0,1),
		extrinsiParams[ Locate::FRONT ].rotationMatrix.Get(1,1),
		extrinsiParams[ Locate::FRONT ].rotationMatrix.Get(2,1),
		extrinsiParams[ Locate::FRONT ].rotationMatrix.Get(0,2),
		extrinsiParams[ Locate::FRONT ].rotationMatrix.Get(1,2),
		extrinsiParams[ Locate::FRONT ].rotationMatrix.Get(2,2) );

	cv::Mat t1 = ( cv::Mat_<double>(3,1) << extrinsiParams[ Locate::FRONT ].translationVector.GetX(),
		extrinsiParams[ Locate::FRONT ].translationVector.GetY(),extrinsiParams[ Locate::FRONT ].translationVector.GetZ() );

	cv::Mat R2 = ( cv::Mat_<double>(3,3) <<
	extrinsiParams[ Locate::RIGHT ].rotationMatrix.Get(0,0),
	extrinsiParams[ Locate::RIGHT ].rotationMatrix.Get(1,0),
	extrinsiParams[ Locate::RIGHT ].rotationMatrix.Get(2,0),
	extrinsiParams[ Locate::RIGHT ].rotationMatrix.Get(0,1),
	extrinsiParams[ Locate::RIGHT ].rotationMatrix.Get(1,1),
	extrinsiParams[ Locate::RIGHT ].rotationMatrix.Get(2,1),
	extrinsiParams[ Locate::RIGHT ].rotationMatrix.Get(0,2),
	extrinsiParams[ Locate::RIGHT ].rotationMatrix.Get(1,2),
	extrinsiParams[ Locate::RIGHT ].rotationMatrix.Get(2,2) );

	cv::Mat t2 = ( cv::Mat_<double>(3,1) << extrinsiParams[ Locate::RIGHT ].translationVector.GetX(),
		extrinsiParams[ Locate::RIGHT ].translationVector.GetY(),
		extrinsiParams[ Locate::RIGHT ].translationVector.GetZ() );


	// ③ 射影行列の生成： P = K * [R | t]
	cv::Mat RT1,RT2,P1,P2;
	hconcat(R1,t1,RT1);
	hconcat(R2,t2,RT2);


	cv::Mat K1 = ( cv::Mat_<double>(3,3) <<
		 instrinsiParams[ Locate::FRONT ].cameraMatrix.Get(0,0),
		instrinsiParams[ Locate::FRONT ].cameraMatrix.Get(1,0),
		instrinsiParams[ Locate::FRONT ].cameraMatrix.Get(2,0),
		 instrinsiParams[ Locate::FRONT ].cameraMatrix.Get(0,1),
		instrinsiParams[ Locate::FRONT ].cameraMatrix.Get(1,1),
		instrinsiParams[ Locate::FRONT ].cameraMatrix.Get(2,1),
		 instrinsiParams[ Locate::FRONT ].cameraMatrix.Get(0,2),
		instrinsiParams[ Locate::FRONT ].cameraMatrix.Get(1,2),
		instrinsiParams[ Locate::FRONT ].cameraMatrix.Get(2,2) );

	cv::Mat K2 = ( cv::Mat_<double>(3,3) <<
	 instrinsiParams[ Locate::RIGHT ].cameraMatrix.Get(0,0),
	instrinsiParams[ Locate::RIGHT ].cameraMatrix.Get(1,0),
	instrinsiParams[ Locate::RIGHT ].cameraMatrix.Get(2,0),
	 instrinsiParams[ Locate::RIGHT ].cameraMatrix.Get(0,1),
	instrinsiParams[ Locate::RIGHT ].cameraMatrix.Get(1,1),
	instrinsiParams[ Locate::RIGHT ].cameraMatrix.Get(2,1),
	 instrinsiParams[ Locate::RIGHT ].cameraMatrix.Get(0,2),
	instrinsiParams[ Locate::RIGHT ].cameraMatrix.Get(1,2),
	instrinsiParams[ Locate::RIGHT ].cameraMatrix.Get(2,2) );

	P1 = K1 * RT1;
	P2 = K2 * RT2;

	// ⑤ 有効な検出のみフィルタリング（信頼性が閾値以上）
	std::vector<cv::Point2f> points1,points2;
	std::vector<int> validIndices;
	for ( size_t i = 0; i < ( int32_t ) YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++ )
	{
		if ( capturedata_[ Locate::FRONT ][ ( YOLO_POSE_INDEX ) i ].captureBonePos.z >= CONFIDENCE_THRESHOLD &&
			 capturedata_[ Locate::RIGHT ][ ( YOLO_POSE_INDEX ) i ].captureBonePos.z >= CONFIDENCE_THRESHOLD )
		{
			cv::Point2f point1 = { capturedata_[ Locate::FRONT ][ ( YOLO_POSE_INDEX ) i ].captureBonePos.x,
									capturedata_[ Locate::FRONT ][ ( YOLO_POSE_INDEX ) i ].captureBonePos.y };

			cv::Point2f point2 = { capturedata_[ Locate::RIGHT ][ ( YOLO_POSE_INDEX ) i ].captureBonePos.x,
									capturedata_[ Locate::RIGHT ][ ( YOLO_POSE_INDEX ) i ].captureBonePos.y };
			points1.push_back(point1);
			points2.push_back(point2);
			validIndices.push_back(static_cast< int >( i ));
		}
	}
	if ( points1.empty() || points2.empty() )
	{
		return;
	}

	// ⑥ 必要に応じた歪み補正
	// ここでは、内部パラメーターを含む射影行列を使っているため、triangulatePointsには元のピクセル座標を使用します。
	// ※ 高精度化のため、事前に undistortPoints を用いて正規化座標に変換する方法もあります。

	// ⑦ 三角測量による3次元復元（OpenCVの triangulatePoints を使用）
	cv::Mat pts4D;
	triangulatePoints(P1,P2,points1,points2,pts4D);

	// ⑧ 同次座標から通常の3次元座標へ変換して出力

	for ( int i = 0; i < pts4D.cols; i++ )
	{
		cv::Mat col = pts4D.col(i);
		// 同次座標（4次元）を第4成分で正規化
		cv::Point3f pt3D(col.at<float>(0,0),
					 col.at<float>(1,0),
					 col.at<float>(2,0));

		finalCaptureData_[ ( YOLO_POSE_INDEX ) validIndices[ i ] ] = MCBO::YVector3(pt3D.x,pt3D.y,-pt3D.z);
	}
}