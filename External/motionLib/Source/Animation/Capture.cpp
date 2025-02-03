#include <Animation/Capture.h>
#include <Math/MathUtil.h>
void MCBM::Capture::Initialize(int32_t index)
{

	capture_ = cv::VideoCapture(index);
	capture_.set(cv::CAP_PROP_FRAME_WIDTH, YOLOPoseEstimation::CAMERA_WITH);
	capture_.set(cv::CAP_PROP_FRAME_HEIGHT, YOLOPoseEstimation::CAMERA_HIGHT);
	capture_.set(cv::CAP_PROP_FPS, 30);
	capture_.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('H', '2', '6', '4'));



	if (!capture_.isOpened())
	{
		initialized_ = false;
		return;
	}

	img_ = cv::imread("Resources\\Rairu.jpg");

	if (img_.empty())
	{
		initialized_ = false;
		return;
	}

	m_YOLOPoseEstimation_->CameraInitialize(&capture_);

	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		CaptureData datatemp;
		capturedata_[(YOLO_POSE_INDEX)i] = datatemp;
	}
	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		CaptureData* temp = &capturedata_[(YOLO_POSE_INDEX)i];
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
			temp->captureChildren.push_back(&capturedata_[YOLO_POSE_INDEX::ELBOW_L]);
			break;
		case YOLO_POSE_INDEX::SHOULDER_R:
			temp->captureChildren.push_back(&capturedata_[YOLO_POSE_INDEX::ELBOW_R]);
			break;
		case YOLO_POSE_INDEX::ELBOW_L:
			temp->captureChildren.push_back(&capturedata_[YOLO_POSE_INDEX::WRIST_L]);
			break;
		case YOLO_POSE_INDEX::ELBOW_R:
			temp->captureChildren.push_back(&capturedata_[YOLO_POSE_INDEX::WRIST_R]);
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

}

void MCBM::Capture::Update()
{

	land_ = m_YOLOPoseEstimation_->GetLandmakes();
	finalPoints_ = m_YOLOPoseEstimation_->GetFinalPositions();

	//skelton構成
	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		capturedata_[(YOLO_POSE_INDEX)i].captureBonePos = { Util::TruncateToTens(land_[i].x),Util::TruncateToTens(land_[i].y),0 };
	}

}

void MCBM::Capture::SetInitialPose()
{
	land_ = m_YOLOPoseEstimation_->GetLandmakes();

	//skelton構成
	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		capturedata_[(YOLO_POSE_INDEX)i].initializedCaptureBonePos = { Util::TruncateToTens(land_[i].x),Util::TruncateToTens(land_[i].y),0 };
		capturedata_[(YOLO_POSE_INDEX)i].captureBonePos = { Util::TruncateToTens(land_[i].x),
			Util::TruncateToTens(land_[i].y),0 };
	}
}

void MCBM::Capture::Finalize()
{
	m_YOLOPoseEstimation_->End();
}

MCBM::CaptureData& MCBM::Capture::GetCaptureData(YOLO_POSE_INDEX key)
{
	return capturedata_[key];
}

MCBM::MVector3& MCBM::Capture::GetFinalPositionData(YOLO_POSE_INDEX key)
{
	MVector3 ret;
	ret.x = finalPoints_->find(key)->second.x;
	ret.y = finalPoints_->find(key)->second.y;
	ret.z = finalPoints_->find(key)->second.z;
	return ret;
}

void MCBM::Capture::SetYOLOEstimation(YOLOPoseEstimation* yoloPoseEst)
{
	m_YOLOPoseEstimation_ = yoloPoseEst;
}
