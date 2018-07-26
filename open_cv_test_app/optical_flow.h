#pragma once

//Optical Flow

static cv::Vec2d Translation(cv::Mat& flow_x, cv::Mat& flow_y)
{
	cv::Vec2d mean_flow_vec;
	mean_flow_vec[0] = cv::mean(flow_x)[0];
	mean_flow_vec[1] = cv::mean(flow_y)[0];
	return mean_flow_vec;
}

static double MeanCurl(cv::Mat& flow_x, cv::Mat& flow_y)
{
	cv::Mat d_dx;
	cv::Mat d_dy;
	cv::Sobel(flow_y, d_dx, CV_64F, 1, 0);
	cv::Sobel(flow_x, d_dy, CV_64F, 0, 1);

	cv::Mat curl = d_dx - d_dy;
	cv::Scalar mean_curl = cv::mean(curl);
	return mean_curl[0];
}


static void GetOpticalFlowVectors(cv::Mat & prior_grayscale, cv::Mat & current_grayscale, cv::Mat& flow_x, cv::Mat& flow_y)
{
	cv::Mat flow;
	cv::calcOpticalFlowFarneback(prior_grayscale, current_grayscale, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

	if (flow_x.empty())
	{
		flow_x = cv::Mat::zeros(flow.size(), CV_64F);
		flow_y = cv::Mat::zeros(flow.size(), CV_64F);
	}

	for (int x = 0; x < flow.rows; x += 1)
	{
		for (int y = 0; y < flow.cols; y += 1)
		{
			const cv::Point2f& fxy = flow.at<cv::Point2f>(x, y);

			flow_x.at<double>(x, y) = fxy.x;
			flow_y.at<double>(x, y) = fxy.y;
		}
	}
}


