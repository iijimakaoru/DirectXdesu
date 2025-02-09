#pragma once
#include <unordered_map>
#include <SimpleVector3.h>
#include <string>
#include "CameraCalibrator.h"

enum class ONNXP_ROVIDERS
{
	CPU,
	CUDA,
	DIRECTML
};

enum class YOLO_POSE_INDEX
{
	NOSE,
	EYE_L,
	EYE_R,
	EAR_L,
	EARR,
	SHOULDER_L,
	SHOULDER_R,
	ELBOW_L,
	ELBOW_R,
	WRIST_L,
	WRIST_R,
	HIP_L,
	HIP_R,
	KNEE_L,
	KNEE_R,
	ANKLE_L,
	ANKLE_R,

	YOLO_POSE_INDEX_MAX
};

enum Locate
{
	FRONT,
	RIGHT,
	MAX_LOCATE
};

struct YOLO_POSE_LANDMAKE
{
	float x;
	float y;
	float vi;
};

class YOLOPoseEstimation
{
public:

	static constexpr float CAMERA_WITH = 480;
	static constexpr float CAMERA_HIGHT = 480;
	static constexpr float CONFIDENCE_THRESHOLD = 0.75f;

	YOLOPoseEstimation() = default;

	virtual ~YOLOPoseEstimation() = default;

	virtual void CameraInitialize(void* cam) = 0;

	virtual void ModelInitialize(const char* modelPath, float mask_threshold = 0.5f, float conf_threshold = 0.30f, float iou_threshold = 0.45f, ONNXP_ROVIDERS provider = ONNXP_ROVIDERS::DIRECTML) = 0;

	virtual void Initialize() = 0;

	virtual void Start(bool isDraw= false) = 0;

	virtual void End() = 0;

	virtual const YOLO_POSE_LANDMAKE* const GetLandmakes(int32_t index = 0) = 0;

	virtual const std::unordered_map <YOLO_POSE_INDEX,MCBO::YVector3>* const GetFinalPositions() = 0;

	virtual void InterinsCalibrateStart(int32_t cameraIndex) = 0;
	virtual void InterinsCalibrateSave(const std::string& filepath) = 0;

	virtual void ExtrinsCalibrateStart(int32_t cameraIndex) = 0;
	virtual void ExtrinsCalibrateSave(const std::string& filepath) = 0;

	virtual void ExtrinsCalibrateLoad(const std::string& filepath) = 0;
	virtual void InterinsCalibrateLoad(const std::string& filepath) = 0;

	virtual void SetCalibrateCallBack(CameraCalibrator::Callback* callBackPtr) = 0;

	virtual const ExtrinsiParameterCalibrator::Parameter GetExtrinsiParameter(int32_t cameraIndex) = 0;
	virtual const IntrinsicParameterCalibrator::Parameter GetInterinsParameter(int32_t cameraIndex) = 0;
};


YOLOPoseEstimation* CreateYOLOPoseEstimation();
