#include "image_processing.h"
#include <iostream>
using std::cout;

ImageProcessing::ImageProcessing(string file, string location, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->img = Img(file, location);

	// TODO: ���ϲ�ͬͼƬ����
	if (img.get_img_type() == "bmp") {
		QImage* img_origin = new QImage;
		img_origin->load(QString::fromStdString(this->img.get_file_path())); //����ͼ��
		ui.originLabel->setPixmap(QPixmap::fromImage(*img_origin)); //ͼ����ʾ��label��
		ui.originLabel->adjustSize(); //�ؼ���Ӧͼ��ע�����ŵ���һ�����֮��
		ui.originScrollArea->setWidget(ui.originLabel); //����labelΪscrollArea�Ĵ���
	}

	else if (img.get_img_type() == "raw") {
		CustomInfo img_info = this->img.get_img_info();
		QImage* img_origin = new QImage(img_info.width, img_info.height, QImage::Format_Grayscale8);
		for (int y = 0; y < img_info.height; y++) {
			for (int x = 0; x < img_info.width; x++) {
				int value = img_info.img_data.at(y * img_info.width + x);
				img_origin->setPixel(x, y, qRgb(value, value, value));
			}
		}
		ui.originLabel->setPixmap(QPixmap::fromImage(*img_origin)); 
		ui.originLabel->adjustSize(); 
		ui.originScrollArea->setWidget(ui.originLabel);
	}

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
	case 3:
		CustomInfo img_info = tools.GrayLevelWindow(img, ui.posLineEdit->text().toInt(), ui.widthLineEdit->text().toInt()).get_img_info();
		QImage* img_dst = new QImage(img_info.width, img_info.height, QImage::Format_Grayscale8);
		for (int y = 0; y < img_info.height; y++) {
			for (int x = 0; x < img_info.width; x++) {
				int value = img_info.img_data.at(y * img_info.width + x);
				img_dst->setPixel(x, y, qRgb(value, value, value));
			}
		}
		ui.dstLabel->setPixmap(QPixmap::fromImage(*img_dst));
		ui.dstLabel->adjustSize();
		ui.dstScrollArea->setWidget(ui.dstLabel);
		break;
	}

	if (img.get_img_type() == "bmp") {
		QImage* img_dst = new QImage;
		img_dst->load(QString::fromStdString(path_temp));
		ui.dstLabel->setPixmap(QPixmap::fromImage(*img_dst));
		ui.dstLabel->adjustSize();
		ui.dstScrollArea->setWidget(ui.dstLabel);
	}
}
