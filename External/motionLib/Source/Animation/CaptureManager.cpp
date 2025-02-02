#include "CaptureManager.h"
#include <Math/Matrix.h>

using namespace MCBM;

void CaptureManager::Initialize()
{
	YOLOPoseEstimation* yolo = CreateYOLOPoseEstimation();

	m_YOLOPoseEstimation_.reset(std::move(yolo));

	m_YOLOPoseEstimation_->ModelInitialize(modelPath_.c_str());


	for ( int32_t i = 0; i < Locate::MAX_LOCATE; i++ )
	{
		capdatas[ i ].SetYOLOEstimation(m_YOLOPoseEstimation_.get());
		capdatas[ i ].Initialize(static_cast< int32_t >( i + cameraFirstIndex));
	}

	m_YOLOPoseEstimation_->Start(true);

	capdatas[ Locate::FRONT ].cameraPosition = { 0.0f,0.0f,capdatas[ Locate::FRONT ].cameraDistance.z };
	//capdatas[ Locate::LEFT ].cameraPosition = { -capdatas[ Locate::LEFT ].cameraDistance.x,0.0f,0.0f };
	//capdatas[ Locate::RIGHT ].cameraPosition = { capdatas[ Locate::RIGHT ].cameraDistance.x,0.0f,0.0f };
}

void CaptureManager::Update()
{
	for ( size_t i = 0; i < Locate::MAX_LOCATE; i++ )
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
	for ( int32_t i = 0; i < Locate::MAX_LOCATE; i++ )
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

	for ( int32_t i = 0; i < Locate::MAX_LOCATE; i++ )
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

