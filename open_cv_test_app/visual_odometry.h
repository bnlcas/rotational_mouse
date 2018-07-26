/*
The MIT License
Copyright (c) 2015 Avi Singh
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core/core.hpp"



//Visual Odometry
void FeatureDetection(cv::Mat img, std::vector<cv::Point2f>& points) {   //uses FAST as of now, modify parameters as necessary
																		 //cv::FeatureDetector *detector = new cv::ORB::Feature2D::detect
	std::vector<cv::KeyPoint> keypoints;
	int fast_threshold = 20;
	bool nonmaxSuppression = true;
	cv::FAST(img, keypoints, fast_threshold, nonmaxSuppression);
	cv::KeyPoint::convert(keypoints, points, std::vector<int>());
}

void FeatureTracking(cv::Mat img_1, cv::Mat img_2, std::vector<cv::Point2f>& points1, std::vector<cv::Point2f>& points2, std::vector<uchar>& status)
{
	//this function automatically gets rid of points for which tracking fails
	std::vector<float> err;
	cv::Size winSize = cv::Size(21, 21);
	cv::TermCriteria termcrit = cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 0.01);

	cv::calcOpticalFlowPyrLK(img_1, img_2, points1, points2, status, err, winSize, 3, termcrit, 0, 0.001);

	//getting rid of points for which the KLT tracking failed or those who have gone outside the frame
	int indexCorrection = 0;
	for (int i = 0; i<status.size(); i++)
	{
		cv::Point2f pt = points2.at(i - indexCorrection);
		if ((status.at(i) == 0) || (pt.x<0) || (pt.y<0)) {
			if ((pt.x<0) || (pt.y<0)) {
				status.at(i) = 0;
			}
			points1.erase(points1.begin() + i - indexCorrection);
			points2.erase(points2.begin() + i - indexCorrection);
			indexCorrection++;
		}
	}
}


double GetZAngle(cv::Mat R)
{
	//double m01 = (R.at<double>(1, 0) - R.at<double>(0, 1));
	//double m21 = R.at<double>(2, 1) - R.at<double>(1, 2);
	//double m20 = R.at<double>(0, 2) - R.at<double>(2, 0);

	//double z = m01 / cv::sqrt(
	//	cv::pow(m21, 2) +
	//	cv::pow(m20, 2) +
	//	cv::pow(m01, 2));

	double z = cv::fastAtan2(R.at<double>(1, 0), R.at<double>(0, 0));//
	return z;
}
