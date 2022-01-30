#include <opencvQuickDemo.h>

using namespace std;

void QuickDemo::colorspace_transform(Mat& image) {
	Mat gray, hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	/*
	1.BGR channels(B:0~255,G:0~255,R:0~255) 
	2.HSV channels(H:0~180, S : 0~255, V : 0~255)
	where HS channels are used to adjust the color
	V channel is used to adjust the luminance
	*/
	cvtColor(image, gray, COLOR_BGR2GRAY);
	namedWindow("HSV", WINDOW_FREERATIO);
	imshow("HSV", hsv);
	namedWindow("GRAY", WINDOW_FREERATIO);
	imshow("GRAY", gray);
	imwrite("D:\\Wallhaven\\hsv_test.png", hsv);
	imwrite("D:\\Wallhaven\\gray_test.png", gray);
}

void QuickDemo::image_create(Mat& image) {
	Mat m1, m2;
	//two methods of copying a image
	m1 = image.clone();
	image.copyTo(m2);//还有一个mask参数，后面掩膜操作会用到

	Mat m3;
	m3 = Mat::zeros(Size(40,40), CV_8UC3);
	m3 = Scalar(80, 90, 100);
	cout << m3 << endl;
	cout << " width: " << m3.cols << " heights:" << m3.rows << " channels: " << m3.channels() << endl;
	//cout << m3 << endl;
	imshow("pic", m3);
}

//visit single pixel and operate on it
void QuickDemo::pixel_visit(Mat& image) {
	int width = image.cols;
	int height = image.rows;
	int dims = image.channels();
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			if (dims == 1) {
				int pv = image.at<uchar>(row, col);
				image.at<uchar>(row, col) = 255 - pv;
			}
			else if (dims == 3) {
				Vec3b bgr = image.at<Vec3b>(row, col);//Vec3b格式适用于一次提取三个数据组成数组
				image.at<Vec3b>(row, col)[0] = 255 - bgr[0];	
				image.at<Vec3b>(row, col)[1] = 255 - bgr[1];
				image.at<Vec3b>(row, col)[2] = 255 - bgr[2];
			}
		}
	}
	namedWindow("pic_remake", WINDOW_FREERATIO);
	imshow("pic_remake", image);
}

void QuickDemo::operators_demo(Mat& image) {
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(50, 50, 50);
	/*这里是add函数的内部实现部分，在使用时直接调用add的API
	adding principle(can be used to enhance the luminance):
	int w = image.cols;
	int h = image.rows;
	int dims = image.channels();
	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			Vec3b p1 = image.at<Vec3b>(row, col);
			Vec3b p2 = m.at<Vec3b>(row, col);
			dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(p1[0] + p2[0]);
			dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(p1[1] + p2[1]);
			dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(p1[2] + p2[2]);
		}
	}
	*/
	subtract(image, m, dst);//减法dst=image-m本质上此条语句是在调整亮度
	/*
	corresponding to the +&-&*&/ operator:
	add(image, m, dst);
	multiply(image, m, dst);
	divide(image, m, dst);
	*/
	namedWindow("after_operate", WINDOW_FREERATIO);
	imshow("after_operate", dst);
}

//the "on_lightness" & "on_contrast" function can add a "static" before "void"
//回调函数
void on_lightness(int b, void* userdata) {
	Mat image = *((Mat*)userdata);
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());
	addWeighted(image, 1.0, m, 0, b, dst);
	imshow("亮度与对比度调整", dst);
}

//回调函数
void on_contrast(int b, void* userdata) {
	Mat image = *((Mat*)userdata);//进行Mat类型指针强制转换
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());
	double contrast = b / 100.0;
	addWeighted(image, contrast, m, 0.0, 0, dst);
	//https://blog.csdn.net/fanjiule/article/details/81607873?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164351142216780271589474%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=164351142216780271589474&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-81607873.pc_search_result_control_group&utm_term=addWeighted&spm=1018.2226.3001.4187
	imshow("亮度与对比度调整", dst);
}

void QuickDemo::tracking_bar_demo(Mat& image) {
	int lightness = 50;
	int max_value = 100;
	int contrast_value = 100;
	namedWindow("亮度与对比度调整", WINDOW_FREERATIO);
	//每当trackbar的数值条有变动的时候就会调用回调函数，而回调函数中的内容就是要对所接受的数据的操作
	createTrackbar("Value Bar:", "亮度与对比度调整", 
		&lightness, max_value, on_lightness, (void*)(&image));
	createTrackbar("Contrast Bar:", "亮度与对比度调整", 
		&contrast_value, 200, on_contrast, (void*)(&image));
	on_lightness(50, &image);
}

//与键盘交互
void QuickDemo::key_respond(Mat& image) {
	Mat dst=Mat::zeros(image.size(),image.type());
	while (true) {
		int c = waitKey(100);//while processing a video it should be "waitKey(1)"
		if (c == 27) {//esc
			break;
		}
		else if (c == 49) {//key#1
			cout << "enter #1" << endl;
			cvtColor(image, dst, COLOR_BGR2GRAY);
		}
		else if (c == 50) {//key#2
			cout << "enter #2" << endl;
			cvtColor(image, dst, COLOR_BGR2HSV);
		}
		else if (c == 51) {//key#3
			cout << "enter #3" << endl;
			dst = Scalar(50, 50, 50);
			add(image, dst, dst);
		}
		imshow("output", dst);
	}
}

//opencv中的自带滤镜库运用
void QuickDemo::color_style_demo(Mat& image) {
	int colormap[] = {
		COLORMAP_AUTUMN,
		COLORMAP_BONE,
		COLORMAP_JET,
		COLORMAP_WINTER,
		COLORMAP_RAINBOW,
		COLORMAP_OCEAN,
		COLORMAP_SUMMER,
		COLORMAP_SPRING,
		COLORMAP_COOL,
		COLORMAP_PINK,
		COLORMAP_HOT,
		COLORMAP_PARULA,
		COLORMAP_MAGMA,
		COLORMAP_INFERNO,
		COLORMAP_PLASMA,
		COLORMAP_VIRIDIS,
		COLORMAP_CIVIDIS,
		COLORMAP_TWILIGHT,
		COLORMAP_TWILIGHT_SHIFTED
	};

	Mat dst;
	int index = 0;
	while (true) {
		int c = waitKey(500);
		if (c == 27) { // 退出
			break;
		}
		applyColorMap(image, dst, colormap[index % 19]);
		index++;
		namedWindow("color_style", WINDOW_FREERATIO);
		imshow("color_style", dst);
	}
}

//对图像的位操作
void QuickDemo::bitwise_demo(Mat& image) {
	Mat m1 = Mat::zeros(Size(10, 10), CV_8UC3);
	Mat m2 = Mat::zeros(Size(10, 10), CV_8UC3);
	//CV_8UC3意思是八位无符号整数三通道格式（用imread函数读取的都是八位格式，通道数可以指定1/2/3)
	Mat dst;
	rectangle(m1, Rect(0, 0, 5, 5), Scalar(255, 255, 0), -1, LINE_8, 0);
	rectangle(m2, Rect(3, 3, 5, 5), Scalar(0, 100, 254), -1, LINE_8, 0);
	/*rectangle函数得倒数第三个参数小于零表示填充，大于零表示绘制边
	Rect(x,y,width,height)，x, y为左上角坐标,width, height则为长和宽*/
	//imshow("m1", m1);
	//imshow("m2", m2);
	bitwise_xor(m1, m2, dst);//还有bitwise_or,bitwise_and，bitwise_not操作
	cout << dst << endl;
	imshow("dst", dst);
}

//通道分离和通道混合操作
void QuickDemo::channels_demo(Mat& image) {
	vector<Mat> mv;
	split(image, mv);//channels seperated
	imshow("蓝色", mv[0]);
	imshow("绿色", mv[1]);
	imshow("红色", mv[2]);

	Mat dst;
	mv[0] = 0;
	// mv[1] = 0;
	merge(mv, dst);
	imshow("红色", dst);

	int from_to[] = { 0, 2, 1,1, 2, 0 };//0-->2,1-->1,2-->0
	/*
	input matrix address, input matrix number, output matrix address, 
	output matrix number,mixed list,mixed channels pairs number
	*/
	mixChannels(&image, 1, &dst, 1, from_to, 3);
	imshow("通道混合", dst);
}

//掩膜操作更换背景
void QuickDemo::inrange_demo(Mat& image) {
	Mat hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	Mat mask;
	inRange(hsv, Scalar(0,0,0), Scalar(180,255,46), mask);
	bitwise_not(mask, mask);
	Mat back=Mat::zeros(image.size(),image.type());
	back = Scalar(40, 220, 40);
	image.copyTo(back, mask);
	namedWindow("after_mask", WINDOW_FREERATIO);
	namedWindow("origin_pic", WINDOW_FREERATIO);
	namedWindow("mask", WINDOW_FREERATIO);
	imshow("origin_pic", image);
	imshow("mask", mask);
	imshow("after_mask", back);
}

//图像数据统计
void QuickDemo::pixel_statistic_demo(Mat& image) {
	double minv, maxv;
	Point minLoc, maxLoc;
	std::vector<Mat> mv;
	split(image, mv);
	//minMaxLoc function only accepts 1 channel, so we need to use split function
	for (int i = 0; i < mv.size(); i++) {
		minMaxLoc(mv[i], &minv, &maxv, &minLoc, &maxLoc, Mat());
		std::cout << "No. channels:" << i << " min value:" << minv << " max value:" << maxv << std::endl;
	}
	Mat mean, stddev;
	//可以利用均值和方差来降噪
	meanStdDev(image, mean, stddev);
	std::cout << "means:" << mean << std::endl;
	std::cout << " stddev:" << stddev << std::endl;
}
