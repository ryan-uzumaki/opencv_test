#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencvQuickDemo.h>

using namespace std;
using namespace cv;


int main() {
	Mat src = imread("D:\\Wallhaven\\23.jpg");
	//namedWindow("input", WINDOW_FREERATIO);
	//imshow("input", src);
	QuickDemo qd;
	//qd.colorspace_transform(src);
	qd.inrange_demo(src);
	//qd.pixel_visit(src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}