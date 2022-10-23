#include "img.h"
#include "processing.h"
#include "message.h"


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
			InputMessage("����������������ȱ��������� ", multiple);
			tools.DisplayEffect(img, tools.GeometricTransform.Scaling(img, multiple));
			break;
		}

		InputMessage("�Ƿ��������0����1���ǣ�", state);
		system("cls");

		if (!state)
			break;
	}
}


// ����Processing�̬ࣨͬ�˲���
void test7() {
	Img img("test.bmp");
	Processing tools;
	tools.DisplayEffect(img, tools.HomomorphicFilter(img));
}


// #include "stdafx.h"
#include <QtWidgets>
#include "gui.h"
#include <QtWidgets/QApplication>


// �����û�����
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	GUI w;
	w.show();
	return a.exec();
}


//int main() {
//	// test6();
//	// test7();
//	// main2();
//}