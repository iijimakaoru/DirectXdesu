#include "YOLOPoseEstimation.h"

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

#include "constants.h"
#include "common.h"
#include "onnx_model_base.h"

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

Vec2F Midpoint(const Vec2F& p1, const Vec2F& p2)
{
	Vec2F mid;
	mid.x = (p1.x + p2.x) / 2.0;
	mid.y = (p1.y + p2.y) / 2.0;
	return mid;
}

Vec2F Translate(const Vec2F& p, const Vec2F& origin)
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

	void ModelInitialize(const char* modelPath, float mask_threshold, float conf_threshold, float iou_threshold, ONNXP_ROVIDERS provider) override;

	void Start(bool isDraw) override;

	const YOLO_POSE_LANDMAKE* const GetLandmakes() override;

	void End() override;

	void Update();

private:

	void _Draw(cv::Mat& image);

private:

	const std::array<Vec2, 19> m_skeleton = { {{16, 14}, {14, 12}, {17, 15}, {15, 13}, {12, 13}, {6, 12}, {7, 13}, {6, 7},{6, 8}, {7, 9}, {8, 10}, {9, 11}, {2, 3}, {1, 2}, {1, 3}, {2, 4}, {3, 5}, {4, 6}, {5, 7} } };
	const std::array<cv::Scalar, 4> m_posePalette = { cv::Scalar(255, 128, 0), cv::Scalar(255, 51, 255), cv::Scalar(51, 153, 255),cv::Scalar(0, 255, 0) };
	const std::array<int32_t, 19> m_limbColorIndices = { 2, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3 };
	const std::array<int32_t, 17> m_kptColorIndices = { 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2 };

	const std::array<std::string, 3> m_provider = { {{"cpu"},{"cuda"},{"directml"}} };

	std::array<YOLO_POSE_LANDMAKE, 17> m_baseLandmakes;
	std::array<YOLO_POSE_LANDMAKE, 17> m_landmakes;
	cv::VideoCapture* m_pCam;

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
};

YOLOPoseEstimation* CreateYOLOPoseEstimation()
{
	static YOLOPoseEstimation* result;

	if (!result)
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
	m_pCam = (cv::VideoCapture*)cam;
}

void YOLOPoseEstimationImp::ModelInitialize(const char* modelPath, float maskThreshold, float confThreshold, float iouThreshold, ONNXP_ROVIDERS provider)
{
	m_modelPath = modelPath;
	m_maskThreshold = maskThreshold;
	m_confhhreshold = confThreshold;
	m_iouThreshold = iouThreshold;

	m_pModel = std::make_unique<AutoBackendOnnx>(m_modelPath.c_str(), m_onnxLogid.c_str(), m_provider[size_t(provider)].c_str());
}

void YOLOPoseEstimationImp::Start(bool isDraw)
{
	m_isDraw = isDraw;
	isRunning = true;

	th = std::thread([this]()
		{
			this->Update();
		});
}

const YOLO_POSE_LANDMAKE* const YOLOPoseEstimationImp::GetLandmakes()
{
	std::lock_guard<std::mutex> lock(value_mutex);
	return m_landmakes.data();
}

void YOLOPoseEstimationImp::End()
{
	isRunning = false;

	th.join();
}

void YOLOPoseEstimationImp::Update()
{
	m_canDraw = false;

	cv::Mat frame;

	while (m_pCam->read(frame) && isRunning)
	{
		std::vector<YoloResults> objs;

		if (m_pModel)
		{
			objs = m_pModel->predict_once(frame, m_confhhreshold, m_iouThreshold, m_maskThreshold);
		}

		if (!objs.empty())
		{
			for (int i = 0; i < (int)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
			{
				int idx = i * 3;
				m_baseLandmakes[i].x = objs[0].keypoints[idx];
				m_baseLandmakes[i].y = objs[0].keypoints[idx + 1];
				m_baseLandmakes[i].vi = objs[0].keypoints[idx + 2];
			}

			Vec2F mid = Midpoint({ m_baseLandmakes[size_t(YOLO_POSE_INDEX::HIP_L)].x, m_baseLandmakes[size_t(YOLO_POSE_INDEX::HIP_L)].y }, { m_baseLandmakes[size_t(YOLO_POSE_INDEX::HIP_R)].x, m_baseLandmakes[size_t(YOLO_POSE_INDEX::HIP_R)].y });

			std::lock_guard<std::mutex> lock(value_mutex);

			for (int i = 0; i < (int)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
			{
				Vec2F newPoint = Translate({ m_baseLandmakes[i].x, m_baseLandmakes[i].y }, mid);

				m_landmakes[i].x = newPoint.x;
				m_landmakes[i].y = newPoint.y;
				m_landmakes[i].vi = m_baseLandmakes[i].vi;
			}

			m_canDraw = true;
		}

		if (m_isDraw)
		{
			_Draw(frame);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	cv::destroyWindow("win");
}

void YOLOPoseEstimationImp::_Draw(cv::Mat& image)
{
	cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
	cv::Size show_shape = image.size();

	if (m_canDraw)
	{
		for (int i = 0; i < m_baseLandmakes.size(); i++)
		{
			if (m_baseLandmakes[i].vi < 0.5)
			{
				continue;
			}

			cv::circle(image, cv::Point(m_baseLandmakes[i].x, m_baseLandmakes[i].y), 5, m_posePalette[m_kptColorIndices[i]], -1, cv::LINE_AA);
		}

		for (int i = 0; i < m_skeleton.size(); i++)
		{
			const Vec2& sk = m_skeleton[i];

			int idx1 = sk.x - 1;
			int idx2 = sk.y - 1;

			int idx1_x_pos = idx1;
			int idx2_x_pos = idx2;

			int x1 = static_cast<int>(m_baseLandmakes[idx1_x_pos].x);
			int y1 = static_cast<int>(m_baseLandmakes[idx1_x_pos].y);
			int x2 = static_cast<int>(m_baseLandmakes[idx2_x_pos].x);
			int y2 = static_cast<int>(m_baseLandmakes[idx2_x_pos].y);

			float conf1 = m_baseLandmakes[idx1_x_pos].vi;
			float conf2 = m_baseLandmakes[idx2_x_pos].vi;

			if (conf1 < 0.5 || conf2 < 0.5)
			{
				continue;
			}

			if (x1 % show_shape.width == 0 || y1 % show_shape.height == 0 || x1 < 0 || y1 < 0 ||
				x2 % show_shape.width == 0 || y2 % show_shape.height == 0 || x2 < 0 || y2 < 0)
			{
				continue;
			}

			cv::Scalar color_limb = m_posePalette[m_limbColorIndices[i]];
			cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2), color_limb, 2, cv::LINE_AA);
		}
	}

	cv::imshow("win", image);
	cv::waitKey(1);
}
