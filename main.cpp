//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
//
//// using namespace cv;
//using namespace std;
//
//
//// ≤‚ ‘opencvµƒ≈‰÷√
//void test1() {
//	cv::Mat img = cv::imread("data/hw1.bmp");
//	cv::namedWindow("≤‚ ‘" );
//	imshow("≤‚ ‘", img);
//	cv::waitKey(0);
//	return;
//}


#include "img.h"

// ≤‚ ‘Img¿‡
void test2() {
	Img img("test.bmp");
	img.DisplayImg();
	img.SaveImg();
}


#include "processing.h"

// ≤‚ ‘Processing¿‡
void test3() {
	Img img("test.bmp");
	Processing tools;
	// tools.FourierTransform(img);
	tools.DisplayEffect(img, tools.HistogramEqualization(img));
}


int main() {
	// test1();
	// test2();
	test3();
}