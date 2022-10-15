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
	tools.DisplayEffect(img, tools.FourierTransform(img));
	// tools.DisplayEffect(img, tools.HistogramEqualization(img));
}


#include "message.h"

// �����û�����
void test4() {
	string location = "data/", file;
	InputMessage("hello world", location, file);
	cout << location + file << endl;
}


// ����Processing�ࣨ���α仯���֣�
void test5() {
	string location = "data/", file = "test.bmp";
	//InputMessage("������ͼƬ�����ļ��е�ַ���ڴ���ͬһĿ¼data�¿�ֱ������data/��", location);
	//InputMessage("��������Ҫ�����ͼƬ�ļ���", file);

	Img img(file, location);
	Processing tools;

	// Mirror
	// tools.DisplayEffect(img, tools.GeometricTransform.Mirror(img));

	// Rotation
	//tools.DisplayEffect(img, tools.GeometricTransform.Rotation(img, -30, 10, 100));

	// Scaling
	float multiple;
	InputMessage("����������������ȱ���������", multiple);
	tools.DisplayEffect(img, tools.GeometricTransform.Scaling(img, multiple));
}


void test6() {
	string location, file;
	float angle, multiple;
	int rx, ry, mode, state;

	InputMessage("������ͼƬ�����ļ��е�ַ���ڴ���ͬһĿ¼data�¿�ֱ������data/��", location);
	InputMessage("��������Ҫ�����ͼƬ�ļ���", file);

	Img img(file, location);
	Processing tools;

	while (true) {
		InputMessage("��ѡ����Ҫ���еĲ�����1��Mirror��2��Rotation��3��Scaling��", mode);
		switch (mode) {
		case 1: 
			tools.DisplayEffect(img, tools.GeometricTransform.Mirror(img));
			break;
		case 2:
			InputMessage("��������ת�Ƕȣ�˳ʱ��Ϊ��ֵ������ת���ģ�x, y����������ת����Ĭ��ֵΪͼƬ���Ŀ����루-1�� -1��", angle, rx, ry);
			tools.DisplayEffect(img, tools.GeometricTransform.Rotation(img, angle, rx, ry));
			break;
		case 3:
			InputMessage("����������������ȱ���������", multiple);
			tools.DisplayEffect(img, tools.GeometricTransform.Scaling(img, multiple));
			break;
		}

		InputMessage("�Ƿ��������0����1���ǣ�", state);
		system("cls");

		if (!state)
			break;
	}
}


int main() {
	// TODO: ���ƺڴ��ڴ�С

	// test1();
	// test2();
	test3();
	// test4();
	// test5();
	// test6();

    // TODO: ��ͣ��system("pause");
}