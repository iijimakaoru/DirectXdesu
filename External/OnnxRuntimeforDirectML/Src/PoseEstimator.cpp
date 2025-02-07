#include "PoseEstimator.h"
	// 広角レンズの歪み補正用パラメータを計算
void PoseEstimator::initializeWideAngleParameters(cv::Mat& camera_matrix,cv::Mat& dist_coeffs) {
		// 焦点距離の計算 (視野角から)
		double fov_rad = FOV * CV_PI / 180.0;
		double focal_length = ( camera_matrix.at<double>(0,2) ) / tan(fov_rad / 2.0);

		// カメラ行列の更新
		camera_matrix.at<double>(0,0) = focal_length;
		camera_matrix.at<double>(1,1) = focal_length;

		// 広角レンズの歪み係数
		// k1, k2, p1, p2, k3, k4, k5, k6
		dist_coeffs = ( cv::Mat_<double>(8,1) <<
			-0.3,0.1,0,0,    // k1, k2, p1, p2 (一般的な値)
			-0.02,0,0,0      // k3, k4, k5, k6 (広角レンズ用)
		);
	}

	// 広角レンズ用の歪み補正
cv::Point2f PoseEstimator::undistortWideAnglePoint(const cv::Point2f& point,const cv::Mat& camera_matrix,const cv::Mat& dist_coeffs)
{
	std::vector<cv::Point2f> src_pts = { point };
	std::vector<cv::Point2f> dst_pts;

	// 画像中心からの距離を計算
	cv::Point2f center(camera_matrix.at<double>(0,2),camera_matrix.at<double>(1,2));
	float radius = cv::norm(point - center);

	// 広角レンズの歪みが強い場合の追加処理
	if ( radius > center.x * 0.8 )
	{  // 画像端に近い場合
// より精密な補正のためのイテレーション回数を増やす
		cv::undistortPoints(src_pts,dst_pts,camera_matrix,dist_coeffs,cv::Mat(),camera_matrix,
						  cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,20,1e-6));
	}
	else
	{
		cv::undistortPoints(src_pts,dst_pts,camera_matrix,dist_coeffs,cv::Mat(),camera_matrix);
	}

	return dst_pts[ 0 ];
}


PoseEstimator::PoseEstimator(
		cv::Mat& cam_matrix1,cv::Mat& dist_coef1,
		cv::Mat& cam_matrix2,cv::Mat& dist_coef2
	) : camera_matrix1(cam_matrix1),dist_coeffs1(dist_coef1),
		camera_matrix2(cam_matrix2),dist_coeffs2(dist_coef2),
		is_calibrated(false) {

		// 広角レンズ用のパラメータ初期化
		initializeWideAngleParameters(camera_matrix1,dist_coeffs1);
		initializeWideAngleParameters(camera_matrix2,dist_coeffs2);

		// 前方カメラの初期化 (-z方向)
		R1 = cv::Mat::eye(3,3,CV_64F);
		t1 = cv::Mat::zeros(3,1,CV_64F);
	}

	// 広角レンズ用のキャリブレーション
	bool PoseEstimator::calibrateCameraPosition(std::vector<cv::Point2f> points1,
							   std::vector<cv::Point2f> points2,
							   float min_confidence = 0.8) {
		if ( points1.size() < 8 || points2.size() < 8 )
		{
			std::cerr << "Not enough points for calibration" << std::endl;
			return false;
		}

		// 広角レンズの歪み補正
		std::vector<cv::Point2f> undist_points1,undist_points2;
		for ( size_t i = 0; i < points1.size(); i++ )
		{
			undist_points1.push_back(undistortWideAnglePoint(points1[ i ],camera_matrix1,dist_coeffs1));
			undist_points2.push_back(undistortWideAnglePoint(points2[ i ],camera_matrix2,dist_coeffs2));
		}

		// 画像端の点のフィルタリング
		std::vector<cv::Point2f> filtered_points1,filtered_points2;
		cv::Point2f center1(camera_matrix1.at<double>(0,2),camera_matrix1.at<double>(1,2));
		cv::Point2f center2(camera_matrix2.at<double>(0,2),camera_matrix2.at<double>(1,2));

		for ( size_t i = 0; i < undist_points1.size(); i++ )
		{
			float radius1 = cv::norm(undist_points1[ i ] - center1);
			float radius2 = cv::norm(undist_points2[ i ] - center2);

			// 画像端（強い歪みがある領域）の点を除外
			if ( radius1 < center1.x * 0.9 && radius2 < center2.x * 0.9 )
			{
				filtered_points1.push_back(undist_points1[ i ]);
				filtered_points2.push_back(undist_points2[ i ]);
			}
		}

		// 対応点が少なすぎる場合はエラー
		if ( filtered_points1.size() < 8 )
		{
			std::cerr << "Not enough valid points after filtering" << std::endl;
			return false;
		}

		// 基本行列の推定（RANSACの閾値を広角レンズ用に調整）
		std::vector<uchar> mask;
		cv::Mat F = cv::findFundamentalMat(filtered_points1,filtered_points2,
										  cv::FM_RANSAC,5.0,0.99,mask);

		// 本質行列の計算
		cv::Mat E = camera_matrix2.t() * F * camera_matrix1;

		// 回転行列と並進ベクトルの抽出
		cv::Mat R,t;
		cv::SVD svd(E);
		cv::Mat W = ( cv::Mat_<double>(3,3) << 0,-1,0,
											 1,0,0,
											 0,0,1 );

		R = svd.u * W * svd.vt;
		t = svd.u.col(2);

		// チェイラリティチェック
		if ( !verifyReconstruction(filtered_points1,filtered_points2,R,t) )
		{
			R = -R;
			t = -t;
		}

		R2 = R;
		t2 = t;
		is_calibrated = true;

		return true;
	}

	// 3D再構成の検証
	bool  PoseEstimator::verifyReconstruction(const std::vector<cv::Point2f>& points1,
							const std::vector<cv::Point2f>& points2,
							const cv::Mat& R,const cv::Mat& t) {
		cv::Mat P1 = cv::Mat::eye(3,4,CV_64F);
		cv::Mat P2 = cv::hconcat(R,t);

		cv::Mat points4D;
		cv::triangulatePoints(P1,P2,points1,points2,points4D);

		int positive_depths = 0;
		for ( int i = 0; i < points4D.cols; i++ )
		{
			if ( points4D.at<float>(2,i) / points4D.at<float>(3,i) > 0 )
			{
				positive_depths++;
			}
		}

		return positive_depths > points4D.cols / 2;
	}

	// 既存のtriangulateメソッドを広角レンズ用に修正
	cv::Point3f  PoseEstimator::triangulate(
		const cv::Point2f& point1,float confidence1,
		const cv::Point2f& point2,float confidence2
	) {
		if ( !is_calibrated || confidence1 < 0.5 || confidence2 < 0.5 )
		{
			return cv::Point3f(0,0,0);
		}

		// 広角レンズの歪み補正
		cv::Point2f undist_point1 = undistortWideAnglePoint(point1,camera_matrix1,dist_coeffs1);
		cv::Point2f undist_point2 = undistortWideAnglePoint(point2,camera_matrix2,dist_coeffs2);

		// 画像端のチェック
		cv::Point2f center1(camera_matrix1.at<double>(0,2),camera_matrix1.at<double>(1,2));
		cv::Point2f center2(camera_matrix2.at<double>(0,2),camera_matrix2.at<double>(1,2));

		if ( cv::norm(undist_point1 - center1) > center1.x * 0.9 ||
			cv::norm(undist_point2 - center2) > center2.x * 0.9 )
		{
			std::cerr << "Point too close to image edge" << std::endl;
			return cv::Point3f(0,0,0);
		}

		cv::Mat P1 = camera_matrix1 * cv::hconcat(R1,t1);
		cv::Mat P2 = camera_matrix2 * cv::hconcat(R2,t2);

		cv::Mat points_4d;
		std::vector<cv::Point2f> points1 = { undist_point1 };
		std::vector<cv::Point2f> points2 = { undist_point2 };

		cv::triangulatePoints(P1,P2,points1,points2,points_4d);

		cv::Mat point_3d = points_4d.col(0);
		point_3d /= point_3d.at<float>(3);

		return cv::Point3f(
			point_3d.at<float>(0),
			point_3d.at<float>(1),
			point_3d.at<float>(2)
		);
	}