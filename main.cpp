#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

// using namespace cv;
using namespace std;


// ≤‚ ‘opencvµƒ≈‰÷√
void test1() {
	cv::Mat img = cv::imread("data/hw1.bmp");
	cv::namedWindow("≤‚ ‘" );
	imshow("≤‚ ‘", img);
	cv::waitKey(0);
	return;
}


#include "img.h"

// ≤‚ ‘Img¿‡
void test2() {
	Img img("hw1.bmp");
}


int main() {
	// test1();
	test2();
}