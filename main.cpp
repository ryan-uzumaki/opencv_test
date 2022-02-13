#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencvQuickDemo.h>

using namespace std;
using namespace cv;


int main() {
	Mat src = imread("C:\\Users\\ryand\\Pictures\\Camera Roll\\WeChat Image_202201301357291.jpg");
	namedWindow("input", WINDOW_FREERATIO);
	imshow("input", src);
	QuickDemo qd;
	//qd.colorspace_transform(src);
	qd.bifilter_demo(src);
	//qd.pixel_visit(src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}