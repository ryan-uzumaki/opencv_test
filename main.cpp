#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencvQuickDemo.h>

using namespace std;
using namespace cv;


int main() {
	Mat src = imread("D:\\Wallhaven\\wallhaven-j5915p_3840x2160.png");
	namedWindow("input", WINDOW_FREERATIO);
	imshow("input", src);
	QuickDemo qd;
	//qd.colorspace_transform(src);
	qd.tracking_bar_demo(src);
	//qd.pixel_visit(src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}