#include "img.h"
#include "processing.h"
#include "message.h"


// ����Processing�̬ࣨͬ�˲���
void test7() {
	Img img("test.bmp");
	Processing tools;
	tools.DisplayEffect(img, tools.HomomorphicFilter(img));
}


//int main() {
//	// test7();
//	// main2();
//	Img img("test.bmp");
//	Processing tools;
//	tools.GeometricTransform.Scaling(img, 1.2).SaveImg("data/temp/test.bmp");
//}

 #include "stdafx.h"
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