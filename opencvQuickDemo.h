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
	void key_respond(Mat& image);
	void color_style_demo(Mat& image);
	void bitwise_demo(Mat& image);
	void channels_demo(Mat& image);
	void inrange_demo(Mat& image);
	void pixel_statistic_demo(Mat& image);
	void drawing_demo(Mat& image);
	void random_drawing();
};