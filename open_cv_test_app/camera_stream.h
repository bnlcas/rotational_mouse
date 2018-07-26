#pragma once

#define CAMERA_STREAM_API __declspec(dllexport)


#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core/core.hpp"


cv::VideoCapture camera_stream(0);
cv::Mat img;
cv::Mat prior_gray_scale;
std::vector<cv::Point2f> prior_key_points;

//Visual Odometry:
cv::Mat R_f, t_f;
cv::Mat E, R, t, mask;
double focal = 500;//718.8560;
cv::Point2d pp(607.1928, 185.2157);//not calibrated

//Optical Flow
cv::Mat flow_x;
cv::Mat flow_y;

//Relevant Variables for a mouse:
double angle;
double x_translation;
double y_translation;

//Interop to send data to a 3d application
extern "C" {
	CAMERA_STREAM_API void InitializeCamera();
}

extern "C" {
	CAMERA_STREAM_API void UpdateCamera();
}

extern "C" {
	CAMERA_STREAM_API double GetRotation();
}

extern "C"
{
	CAMERA_STREAM_API double XTranslation();
}

extern "C"
{
	CAMERA_STREAM_API double YTranslation();
}


