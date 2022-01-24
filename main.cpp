#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencvQuickDemo.h>

using namespace std;
using namespace cv;


int main() {
	Mat src = imread("D:\\Wallhaven\\wallhaven-j5915p_3840x2160.png");
	namedWindow("input", WINDOW_FREERATIO);
	cout << src << endl;
	imshow("input", src);
	QuickDemo qd;
	qd.colorspace_transform(src);
	qd.image_create(src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}