#include "image_processing.h"
#include <iostream>
using std::cout;

ImageProcessing::ImageProcessing(string file, string location, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->img = Img(file, location);

	QImage* img_origin = new QImage;
	img_origin->load(QString::fromStdString(this->img.get_file_path())); //����ͼ��
	ui.originLabel->setPixmap(QPixmap::fromImage(*img_origin)); //ͼ����ʾ��label��
	ui.originLabel->adjustSize(); //�ؼ���Ӧͼ��ע�����ŵ���һ�����֮��
	ui.originScrollArea->setWidget(ui.originLabel); //����labelΪscrollArea�Ĵ���

	system("md data\\temp");
}

ImageProcessing::~ImageProcessing() {
	system("rd /s /q data\\temp");
}

void ImageProcessing::set_mode() {
	int counts = 0;
	if (this->ui.rotationCheckBox->isChecked()) {
		counts++;
		this->mode = 1;
	}
	if (this->ui.scalingCheckBox->isChecked()) {
		counts++;
		this->mode = 2;
	}
	if (this->ui.mappingCheckBox->isChecked()) {
		counts++;
		this->mode = 3;
	}
	// TODO: ��������� �� 
	//if (counts != 1) {
	//	QMessageBox::critical()
	//}
}

void ImageProcessing::startProcessing() {
	QImage* img_dst = new QImage;
	string path_temp;

	set_mode();

	switch (this->mode) {
	case 1:
		path_temp = "data/temp/1." + this->img.get_img_type();
		tools.GeometricTransform.Rotation(img, ui.angleLineEdit->text().toFloat(), ui.rxLineEdit->text().toInt(), ui.ryLineEdit->text().toInt()).SaveImg(path_temp);
		break;
	case 2:
		path_temp = "data/temp/2." + this->img.get_img_type();
		tools.GeometricTransform.Scaling(img, ui.multipleLineEdit->text().toFloat()).SaveImg(path_temp);
		break;
	}

	img_dst->load(QString::fromStdString(path_temp));
	ui.dstLabel->setPixmap(QPixmap::fromImage(*img_dst));
	ui.dstLabel->adjustSize();
	ui.dstScrollArea->setWidget(ui.dstLabel);
}
