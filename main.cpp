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
	tools.FourierTransform(img);
	tools.DisplayEffect(img, tools.HistogramEqualization(img));
}


#include "message.h"

// 测试用户交互
void test4() {
	string location = "data/", file;

	PrintHintMessageB("请输入图片所在文件夹地址（在代码同一目录data下可直接输入data/）");
	cin >> location;
	PrintHintMessageA();

	PrintHintMessageB("请输入需要处理的图片文件名");
	cin >> file;
	PrintHintMessageA();

	Img img(file, location);
	Processing tools;

	tools.DisplayEffect(img, tools.HistogramEqualization(img));
}


// 测试Processing类（几何变化部分）
void test5() {
	string location = "data/", file = "test.bmp";

	//PrintHintMessageB("请输入图片所在文件夹地址（在代码同一目录data下可直接输入data/）");
	//cin >> location;
	//PrintHintMessageA();

	//PrintHintMessageB("请输入需要处理的图片文件名");
	//cin >> file;
	//PrintHintMessageA();

	Img img(file, location);
	Processing tools;

	// Mirror
	// tools.DisplayEffect(img, tools.GeometricTransform.Mirror(img));

	// Scaling
	PrintHintMessageB("请输入放缩倍数（等比例放缩）");
	float multiple;
	cin >> multiple;
	PrintHintMessageA();
	tools.DisplayEffect(img, tools.GeometricTransform.Scaling(img, multiple));

}


int main() {
	// TODO: 控制黑窗口大小

	// test1();
	// test2();
	// test3();
	test5();

    // TODO: 暂停，system("pause");
}