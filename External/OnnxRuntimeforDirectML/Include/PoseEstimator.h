#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
class PoseEstimator
{
	cv::Mat camera_matrix1;
	cv::Mat dist_coeffs1;
	cv::Mat camera_matrix2;
	cv::Mat dist_coeffs2;
	cv::Mat R1,t1;
	cv::Mat R2,t2;
	bool is_calibrated;
	const double FOV = 120.0; // 視野角（度）
	void initializeWideAngleParameters(cv::Mat& camera_matrix,cv::Mat& dist_coeffs);
public:
	cv::Point2f undistortWideAnglePoint(const cv::Point2f& point,const cv::Mat& camera_matrix,const cv::Mat& dist_coeffs);
	PoseEstimator(cv::Mat& cam_matrjix1,cv::Mat& dist_coef1,cv::Mat& cam_matrix2,cv::Mat& dist_coef2);

	bool calibrateCameraPosition(std::vector<cv::Point2f> points1,std::vector<cv::Point2f> points2,float min_confidence);

	bool verifyReconstruction(const std::vector<cv::Point2f>& points1,const std::vector<cv::Point2f>& points2,const cv::Mat& R,const cv::Mat& t);

	cv::Point3f triangulate(const cv::Point2f& point1,float confidence1,const cv::Point2f& point2,float confidence2);

};

