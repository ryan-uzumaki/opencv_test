
//打开摄像头样例
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
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
	qd.colorspace_transform(src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}