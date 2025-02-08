#include "CaptureManager.h"
#include <Math/Matrix.h>

using namespace MCBM;

MCBM::CaptureManager::~CaptureManager()
{
}

MCBM::CaptureManager::CaptureManager()
{
}


CaptureManager* MCBM::CaptureManager::GetInstance()
{
	static CaptureManager inst;
	return &inst;
}

void CaptureManager::Initialize(int32_t cameraNum)
{
	if (initialized)
	{
		return;
	}
	cameraNum_ = cameraNum;

	if (cameraNum > Locate::MAX_LOCATE)
	{
		cameraNum = Locate::MAX_LOCATE;
	}

	YOLOPoseEstimation* yolo = CreateYOLOPoseEstimation();

	m_YOLOPoseEstimation_.reset(std::move(yolo));

	m_YOLOPoseEstimation_->ModelInitialize(modelPath_.c_str());


	for ( int32_t i = 0; i < cameraNum_; i++ )
	{
		capdatas[ i ].SetYOLOEstimation(m_YOLOPoseEstimation_.get());
		capdatas[ i ].Initialize(static_cast< int32_t >( i + cameraFirstIndex));
	}

	m_YOLOPoseEstimation_->Start(true);


	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		finalCaptureData_[(YOLO_POSE_INDEX)i].captureBoneName = 
										capdatas[0].GetCaptureData((YOLO_POSE_INDEX) i).captureBoneName;

	}

	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		CaptureData* temp = &finalCaptureData_[(YOLO_POSE_INDEX)i];
		temp->captureBoneName = linkBoneNames[i];
		switch ((YOLO_POSE_INDEX)i)
		{
		case YOLO_POSE_INDEX::NOSE:

			break;
		case YOLO_POSE_INDEX::EYE_L:

			break;
		case YOLO_POSE_INDEX::EYE_R:

			break;
		case YOLO_POSE_INDEX::EAR_L:

			break;
		case YOLO_POSE_INDEX::EARR:

			break;
		case YOLO_POSE_INDEX::SHOULDER_L:
			temp->captureChildren.push_back(&finalCaptureData_[YOLO_POSE_INDEX::ELBOW_L]);
			break;
		case YOLO_POSE_INDEX::SHOULDER_R:
			temp->captureChildren.push_back(&finalCaptureData_[YOLO_POSE_INDEX::ELBOW_R]);
			break;
		case YOLO_POSE_INDEX::ELBOW_L:
			temp->captureChildren.push_back(&finalCaptureData_[YOLO_POSE_INDEX::WRIST_L]);
			break;
		case YOLO_POSE_INDEX::ELBOW_R:
			temp->captureChildren.push_back(&finalCaptureData_[YOLO_POSE_INDEX::WRIST_R]);
			break;
		case YOLO_POSE_INDEX::WRIST_L:

			break;
		case YOLO_POSE_INDEX::WRIST_R:

			break;
		case YOLO_POSE_INDEX::HIP_L:

			break;
		case YOLO_POSE_INDEX::HIP_R:

			break;
		case YOLO_POSE_INDEX::KNEE_L:

			break;
		case YOLO_POSE_INDEX::KNEE_R:

			break;
		case YOLO_POSE_INDEX::ANKLE_L:

			break;
		case YOLO_POSE_INDEX::ANKLE_R:

			break;
		default:
			break;
		}
	}


	capdatas[ Locate::FRONT ].cameraPosition = { 0.0f,0.0f,1.0f };
	//capdatas[ Locate::LEFT ].cameraPosition = { -capdatas[ Locate::LEFT ].cameraDistance.x,0.0f,0.0f };
	capdatas[ Locate::RIGHT ].cameraPosition = { 1.0f,0.0f,0.0f };
	initialized = true;
}

void CaptureManager::Update()
{
	for ( size_t i = 0; i < cameraNum_; i++ )
	{
		capdatas[ i ].Update();
	}


	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		finalCaptureData_[(YOLO_POSE_INDEX)i].captureBonePos = capdatas[0].GetFinalPositionData((YOLO_POSE_INDEX)i);
	}

}


void CaptureManager::InitializePose()
{
	for ( int32_t i = 0; i < cameraNum_; i++ )
	{
		capdatas[ i ].Update();
	}


	for ( int32_t i = 0; i < ( int32_t ) YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++ )
	{
		finalCaptureData_[(YOLO_POSE_INDEX)i].captureBonePos = capdatas[0].GetFinalPositionData((YOLO_POSE_INDEX)i);
		finalCaptureData_[ ( YOLO_POSE_INDEX ) i ].initializedCaptureBonePos
			= finalCaptureData_[ ( YOLO_POSE_INDEX ) i ].captureBonePos;
	}


}

void MCBM::CaptureManager::Finalize()
{

	for ( int32_t i = 0; i < cameraNum_; i++ )
	{
		capdatas[ i ].Finalize();
	}
}

CaptureData& CaptureManager::GetCaptureData(YOLO_POSE_INDEX key)
{
	return finalCaptureData_[ key ];
}

CaptureData& MCBM::CaptureManager::GetLocateCaptureData(YOLO_POSE_INDEX key,Locate locate)
{
	return capdatas[ (int32_t)locate ].GetCaptureData(key);
}

YOLOPoseEstimation* MCBM::CaptureManager::GetYOLOPoseEstimation()
{
	return m_YOLOPoseEstimation_.get();
}

