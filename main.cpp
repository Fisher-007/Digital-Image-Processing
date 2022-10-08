//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
//
//// using namespace cv;
//using namespace std;
//
//
//// ����opencv������
//void test1() {
//	cv::Mat img = cv::imread("data/hw1.bmp");
//	cv::namedWindow("����" );
//	imshow("����", img);
//	cv::waitKey(0);
//	return;
//}


#include "img.h"

// ����Img��
void test2() {
	Img img("test.bmp");
	img.DisplayImg();
	img.SaveImg();
}


#include "processing.h"

// ����Processing�ࣨ����Ҷ�任+ֱ��ͼ���⻯��
void test3() {
	Img img("test.bmp");
	Processing tools;
	tools.FourierTransform(img);
	tools.DisplayEffect(img, tools.HistogramEqualization(img));
}


#include "message.h"

// �����û�����
void test4() {
	string location = "data/", file;

	PrintHintMessageB("������ͼƬ�����ļ��е�ַ���ڴ���ͬһĿ¼data�¿�ֱ������data/��");
	cin >> location;
	PrintHintMessageA();

	PrintHintMessageB("��������Ҫ�����ͼƬ�ļ���");
	cin >> file;
	PrintHintMessageA();

	Img img(file, location);
	Processing tools;

	tools.DisplayEffect(img, tools.HistogramEqualization(img));
}


// ����Processing�ࣨ���α仯���֣�
void test5() {
	string location = "data/", file = "test.bmp";

	//PrintHintMessageB("������ͼƬ�����ļ��е�ַ���ڴ���ͬһĿ¼data�¿�ֱ������data/��");
	//cin >> location;
	//PrintHintMessageA();

	//PrintHintMessageB("��������Ҫ�����ͼƬ�ļ���");
	//cin >> file;
	//PrintHintMessageA();

	Img img(file, location);
	Processing tools;

	// Mirror
	// tools.DisplayEffect(img, tools.GeometricTransform.Mirror(img));

	// Scaling
	PrintHintMessageB("����������������ȱ���������");
	float multiple;
	cin >> multiple;
	PrintHintMessageA();
	tools.DisplayEffect(img, tools.GeometricTransform.Scaling(img, multiple));

}


int main() {
	// TODO: ���ƺڴ��ڴ�С

	// test1();
	// test2();
	// test3();
	test5();

    // TODO: ��ͣ��system("pause");
}