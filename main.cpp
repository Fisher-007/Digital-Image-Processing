#include "img.h"
#include "processing.h"
#include "message.h"


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
			InputMessage("请输入放缩倍数（等比例放缩） ", multiple);
			tools.DisplayEffect(img, tools.GeometricTransform.Scaling(img, multiple));
			break;
		}

		InputMessage("是否继续处理（0：否，1：是）", state);
		system("cls");

		if (!state)
			break;
	}
}


// 测试Processing类（同态滤波）
void test7() {
	Img img("test.bmp");
	Processing tools;
	tools.DisplayEffect(img, tools.HomomorphicFilter(img));
}


// #include "stdafx.h"
#include <QtWidgets>
#include "gui.h"
#include <QtWidgets/QApplication>


// 测试用户界面
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