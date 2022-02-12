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
	add(image, m, dst);与减法相同也是再调整亮度 dst=image+m
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
	/*rectangle函数是一个矩形绘制函数，它的倒数第三个参数小于零表示填充，大于零表示绘制边时的线宽
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
	inRange(hsv, Scalar(11,43,46), Scalar(25,255,255), mask);
	bitwise_not(mask, mask);
	Mat back=Mat::zeros(image.size(),image.type());
	back = Scalar(220, 40, 40);
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

void QuickDemo::drawing_demo(Mat& image) {
	Rect rect;//创建rect对象，若不分开设定参数则可以用之前rectangle方法中的Rect()函数
	rect.x = 100;
	rect.y = 100;
	rect.width = 250;
	rect.height = 300;
	Mat bg = Mat::zeros(image.size(), image.type());
	rectangle(bg, rect, Scalar(0, 0, 255), -1, 8, 0);
	circle(bg, Point(350, 400), 15, Scalar(255, 0, 0), -1, 8, 0);
	//point()是指定中心点，15是半径
	line(bg, Point(100, 100), Point(350, 400), Scalar(0, 255, 0), 4, LINE_AA, 0);
	//有两个point()函数，第一个是指定起始点，第二个是指定终点
	RotatedRect rrt;
	rrt.center = Point(200, 200);
	rrt.size = Size(100, 200);
	rrt.angle = 90.0;
	ellipse(bg, rrt, Scalar(0, 255, 255), 2, 8);
	Mat dst;
	addWeighted(image, 0.7, bg, 0.3, 0, dst);
	imshow("绘制演示", bg);
}

void QuickDemo::random_drawing() {
	Mat canvas = Mat::zeros(Size(512, 512), CV_8UC3);
	int w = canvas.cols;
	int h = canvas.rows;
	RNG rng(1234567);
	while (true) {
		int c = waitKey(10);
		if (c == 27) { // 退出
			break;
		}
		int x1 = rng.uniform(0, w);
		int y1 = rng.uniform(0, h);
		int x2 = rng.uniform(0, w);
		int y2 = rng.uniform(0, h);
		int b = rng.uniform(0, 255);
		int g = rng.uniform(0, 255);
		int r = rng.uniform(0, 255);
		canvas = Scalar(0, 0, 0);
		line(canvas, Point(x1, y1), Point(x2, y2), Scalar(b, g, r), 1, LINE_AA, 0);
		imshow("随机绘制演示", canvas);
	}
}

void QuickDemo::polyline_drawing_demo() {
	Mat canvas = Mat::zeros(Size(512, 512), CV_8UC3);
	int w = canvas.cols;
	int h = canvas.rows;
	Point p1(100, 100);
	Point p2(300, 150);
	Point p3(300, 350);
	Point p4(250, 450);
	Point p5(50, 450);
	std::vector<Point> pts;
	pts.push_back(p1);
	pts.push_back(p2);
	pts.push_back(p3);
	pts.push_back(p3);
	pts.push_back(p4);
	pts.push_back(p5);
	// polylines(canvas, pts, true, Scalar(0, 255, 0), -1, 8, 0);
	std::vector<std::vector<Point>> contours;//创建点集向量contours
	contours.push_back(pts);
	//【drawoContours(画布，点集对象，点集对象索引（-1表示一次性绘制全部），颜色，绘制方式（-1为填充），线形（LINE_AA为反锯齿,该参数默认为8））】
	drawContours(canvas, contours, 0, Scalar(0, 0, 255), -1, 8);
	imshow("绘制多边形", canvas);
}

Point sp(-1, -1);
Point ep(-1, -1);
Mat temp;
static void onMouse(int event, int x, int y, int flags, void* userdata) {
	Mat image = *((Mat*)userdata);
	if (event == EVENT_LBUTTONDOWN) {
		sp.x = x;
		sp.y = y;
		cout << "start_point: " << sp << endl;
	}
	else if (event == EVENT_LBUTTONUP) {
		ep.x = x;
		ep.y = y;
		int dx = ep.x - sp.x;
		int dy = ep.y - sp.y;
		if ((dx > 0) && (dy > 0)) {
			Rect box(sp.x, sp.y, dx, dy);
			temp.copyTo(image);
			namedWindow("ROI_area", WINDOW_FREERATIO);
			imshow("ROI_area", image(box));
			rectangle(image, box, Scalar(0, 0, 255), 2, 8, 0);
			namedWindow("mouse_behaviour", WINDOW_FREERATIO);
			imshow("mouse_behabiour", image);
			sp.x = -1;
			sp.y = -1;
		}
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (sp.x > 0 && sp.y > 0) {
			ep.x = x;
			ep.y = y;
			int dx = ep.x - sp.x;
			int dy = ep.y - sp.y;
			if (dx > 0 && dy > 0) {
				Rect box(sp.x, sp.y, dx, dy);
				temp.copyTo(image);
				namedWindow("mouse_behaviour", WINDOW_FREERATIO);
				rectangle(image, box, Scalar(0, 0, 255), 2, 8, 0);
				imshow("mouse_behabiour", image);
			}
		}
	}
}
void QuickDemo::mouse_drawing_demo(Mat& image) {
	namedWindow("mouse_behaviour", WINDOW_FREERATIO);
	temp = image.clone();
	setMouseCallback("mouse_behaviour", onMouse, (void*)(&image));
	imshow("mouse_behaviour", image);
}

void QuickDemo::norm_demo(Mat& image) {
	Mat dst;
	std::cout << image.type() << std::endl;
	image.convertTo(image, CV_32F);//转成32位的浮点数CV_32FC3
	std::cout << image.type() << std::endl;
	normalize(image, dst, 1.0, 0, NORM_MINMAX);//NORM_MINMAX最为常用，在做norm分布是一定要先将图像转变为浮点数
	std::cout << dst.type() << std::endl;
	imshow("图像数据归一化", dst);//用imshow（）来显示浮点数图像的时候必须归一化为零到一之间的数字
	// CV_8UC3, CV_32FC3
}

//图像放大缩小操作
static void onmouse_callback(int event, int x, int y, int flags, void* userdata) {
	Mat image = *(Mat*)userdata;
	Mat zoomin;
	long h = image.rows;
	long w = image.cols;
	int value;
	if (event == EVENT_MOUSEWHEEL) {
		value = getMouseWheelDelta(flags);
		if (value > 0) {
			resize(image, zoomin, Size(w * 1.5, h * 1.5), 0, 0, INTER_LINEAR);
			//imshow("resize", zoomin);
			w *= 1.5;
			h *= 1.5;
		}
		else if (value < 0) {
			resize(image, zoomin, Size(w / 1.5, h / 1.5), 0, 0, INTER_LINEAR);
			//imshow("resize", zoomin);
			w /= 1.5;
			h /= 1.5;
		}
		if (!image.empty())
		{
			imshow("resize", zoomin);;
		}
	}
}
void QuickDemo::resize_demo(Mat& image) {
	namedWindow("resize",WINDOW_AUTOSIZE);
	imshow("resize", image);
	setMouseCallback("resize", onmouse_callback, (void*)(&image));
	/*resize函数第四与第五个参数在第三个参数为Size（0，0）时才会用到，
	其值分别为x方向与y方向的长度resize(image, zoomin, Size(w / 2, h / 2), 0, 0, INTER_LINEAR);
	INTER_LINEAR方式运行最为快捷*/
}

//对称处理
void QuickDemo::flip_demo(Mat& image) {
	Mat dst;
	// flip(image, dst, 0); // 上下翻转
	// flip(image, dst, 1); // 左右翻转
	flip(image, dst, -1); // 180°旋转
	imshow("图像翻转", dst);
}

//图像旋转操作
void QuickDemo::rotate_demo(Mat& image) {
	Mat dst, M;
	int w = image.cols;
	int h = image.rows;
	//getRotationMatrix2D()函数第一个参数是源图像的中心点，第二个参数是旋转角度，第三个是缩放比例
	M = getRotationMatrix2D(Point2f(w / 2, h / 2), 45, 1.0);
	double cos = abs(M.at<double>(0, 0));
	double sin = abs(M.at<double>(0, 1));
	int nw = cos * w + sin * h;//新画布的宽
	int nh = sin * w + cos * h;//新画布的高
	//下面两句是调整平移参数，使旋转过后的图像信息不损失
	//参考链接 https://cloud.tencent.com/developer/article/1798209
	M.at<double>(0, 2) += (nw / 2 - w / 2);
	M.at<double>(1, 2) += (nh / 2 - h / 2);
	warpAffine(image, dst, M, Size(nw, nh), INTER_LINEAR, 0, Scalar(255, 255, 0));
	imshow("旋转演示", dst);
}

void QuickDemo::video_demo(Mat& image) {
	VideoCapture capture(0);
	int frame_width = capture.get(CAP_PROP_FRAME_WIDTH);//获取视频宽度
	int frame_height = capture.get(CAP_PROP_FRAME_HEIGHT);//获取视频高度
	int count = capture.get(CAP_PROP_FRAME_COUNT);//获取总帧数
	double fps = capture.get(CAP_PROP_FPS);//获取帧数
	std::cout << "frame width:" << frame_width << std::endl;
	std::cout << "frame height:" << frame_height << std::endl;
	std::cout << "FPS:" << fps << std::endl;
	std::cout << "Number of Frames:" << count << std::endl;
	//用writer对象写入视频的时候要注意写入视频的size要和capture的size是一样的
	VideoWriter writer("D:/test.mp4", capture.get(CAP_PROP_FOURCC), fps, Size(frame_width, frame_height), true);
	Mat frame;
	while (true) {
		capture.read(frame);
		if (frame.empty()) {
			break;
		}
		//这下面直到imshow之前都可以结合之前的函数对每一帧图像进行处理
		flip(frame, frame, 1);
		writer.write(frame);
		imshow("capture", frame);
		int c = waitKey(1);
		if (c == 27) {
			break;
		}
	}
	capture.release();
}