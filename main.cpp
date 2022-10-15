//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
//
//// using namespace cv;
//using namespace std;
//
//
//// 测试opencv的配置
//void test1() {
//	cv::Mat img = cv::imread("data/hw1.bmp");
//	cv::namedWindow("测试" );
//	imshow("测试", img);
//	cv::waitKey(0);
//	return;
//}


#include "img.h"

// 测试Img类
void test2() {
	Img img("test.bmp");
	img.DisplayImg();
	img.SaveImg();
}


#include "processing.h"

// 测试Processing类（傅里叶变换+直方图均衡化）
void test3() {
	Img img("test.bmp");
	Processing tools;
	tools.DisplayEffect(img, tools.FourierTransform(img));
	// tools.DisplayEffect(img, tools.HistogramEqualization(img));
}


#include "message.h"

// 测试用户交互
void test4() {
	string location = "data/", file;
	InputMessage("hello world", location, file);
	cout << location + file << endl;
}


// 测试Processing类（几何变化部分）
void test5() {
	string location = "data/", file = "test.bmp";
	//InputMessage("请输入图片所在文件夹地址（在代码同一目录data下可直接输入data/）", location);
	//InputMessage("请输入需要处理的图片文件名", file);

	Img img(file, location);
	Processing tools;

	// Mirror
	// tools.DisplayEffect(img, tools.GeometricTransform.Mirror(img));

	// Rotation
	//tools.DisplayEffect(img, tools.GeometricTransform.Rotation(img, -30, 10, 100));

	// Scaling
	float multiple;
	InputMessage("请输入放缩倍数（等比例放缩）", multiple);
	tools.DisplayEffect(img, tools.GeometricTransform.Scaling(img, multiple));
}


void test6() {
	string location, file;
	float angle, multiple;
	int rx, ry, mode, state;

	InputMessage("请输入图片所在文件夹地址（在代码同一目录data下可直接输入data/）", location);
	InputMessage("请输入需要处理的图片文件名", file);

	Img img(file, location);
	Processing tools;

	while (true) {
		InputMessage("请选择想要进行的操作（1：Mirror，2：Rotation，3：Scaling）", mode);
		switch (mode) {
		case 1: 
			tools.DisplayEffect(img, tools.GeometricTransform.Mirror(img));
			break;
		case 2:
			InputMessage("请输入旋转角度（顺时针为正值）和旋转中心（x, y），其中旋转中心默认值为图片中心可输入（-1， -1）", angle, rx, ry);
			tools.DisplayEffect(img, tools.GeometricTransform.Rotation(img, angle, rx, ry));
			break;
		case 3:
			InputMessage("请输入放缩倍数（等比例放缩）", multiple);
			tools.DisplayEffect(img, tools.GeometricTransform.Scaling(img, multiple));
			break;
		}

		InputMessage("是否继续处理（0：否，1：是）", state);
		system("cls");

		if (!state)
			break;
	}
}


int main() {
	// TODO: 控制黑窗口大小

	// test1();
	// test2();
	test3();
	// test4();
	// test5();
	// test6();

    // TODO: 暂停，system("pause");
}