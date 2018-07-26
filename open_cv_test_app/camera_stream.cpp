#include "camera_stream.h"
#include "visual_odometry.h"
#include "optical_flow.h"

#include <iostream>
#include <opencv2\highgui\highgui.hpp>

//Camera Setup
extern "C" {
	void InitializeCamera()
	{
		camera_stream.read(img);
		cv::cvtColor(img, prior_gray_scale, CV_BGR2GRAY);

		FeatureDetection(prior_gray_scale, prior_key_points);
	}
}

//Update Pose
extern "C" {
	void UpdateCamera()
	{
		camera_stream.read(img);
		cv::Mat gray_scale;
		cv::cvtColor(img, gray_scale, CV_BGR2GRAY);

		std::vector<cv::Point2f> new_key_points;
		FeatureDetection(gray_scale, new_key_points);

		try
		{
			std::vector<uchar> status;
			FeatureTracking(prior_gray_scale, gray_scale, prior_key_points, new_key_points, status);

			cv::Mat E = cv::findEssentialMat(prior_key_points, new_key_points, focal, pp, cv::RANSAC, 0.999, 0.2, mask);
			cv::recoverPose(E, new_key_points, prior_key_points, R, t, focal, pp, mask);

			angle = GetZAngle(R);
			x_translation = t.at<double>(0, 0);
			y_translation = t.at<double>(0, 1);

			prior_gray_scale = gray_scale;
			prior_key_points = new_key_points;
		}
		catch (std::exception e)
		{
			//If pose estimation breaks, reinitialize
			InitializeCamera();
		}


		//Optical Flow based rotation
		//GetOpticalFlowVectors(prior_gray_scale, gray_scale, flow_x, flow_y);
		//angle = MeanCurl(flow_x, flow_y);
		//cv::Vec2d xy_translation = Translation(flow_x, flow_y);
		//x_translation = xy_translation[0];
		//y_translation = xy_translation[1];	
		//prior_gray_scale = gray_scale;
	}
}

//Functions to get relevant data from CV tracker
extern "C" {
	double GetRotation()
	{
		return angle;
	}
}

extern "C"
{
	double XTranslation()
	{
		return x_translation;
	}
}

extern "C"
{
	double YTranslation()
	{
		return y_translation;
	}
}

//int main()
//{
//	InitializeCamera();
//	std::cout << "Initialized";
//
//	UpdateCamera();
//	std::cout << "n_keyPoints: " << prior_key_points.size();
//	int ind = 0;
//	while (camera_stream.isOpened())
//	{
//		UpdateCamera();
//		ind += 1;
//		if (ind % 10 == 0)
//		{
//			std::cout << "X_trans: " << x_translation << " Y_trans: " << y_translation << " Z_Trans: " << t.at<double>(0, 2) << " ZAngle: " << angle << "\n";
//		}
//	}
//}
