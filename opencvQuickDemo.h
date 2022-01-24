#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;

class QuickDemo {
public:
	void colorspace_transform(Mat& image);
	void image_create(Mat& image);
	void pixel_visit(Mat& image);
	void operators_demo(Mat& image);
	void tracking_bar_demo(Mat& image);
};