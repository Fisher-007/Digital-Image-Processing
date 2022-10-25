#include "img.h"
#include "processing.h"
#include "message.h"


// 测试Processing类（同态滤波）
void test7() {
	Img img("test.bmp");
	Processing tools;
	tools.DisplayEffect(img, tools.HomomorphicFilter(img));
}

// 测试CustomImg类
void test8() {
	Img img("lung.raw");
	img.SaveImg();
}


//int main() {
//	// test7();
//	// main2();
//	test8();
//}

 #include "stdafx.h"
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