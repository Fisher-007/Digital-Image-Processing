#include "image_processing.h"
#include <iostream>
using std::cout;

ImageProcessing::ImageProcessing(string file, string location, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->img = Img(file, location);

	// TODO: 整合不同图片类型
	if (img.get_img_type() == "bmp") {
		QImage* img_origin = new QImage;
		img_origin->load(QString::fromStdString(this->img.get_file_path())); //载入图像
		ui.originLabel->setPixmap(QPixmap::fromImage(*img_origin)); //图像显示在label上
		ui.originLabel->adjustSize(); //控件适应图像（注意必须放到上一句代码之后）
		ui.originScrollArea->setWidget(ui.originLabel); //设置label为scrollArea的窗帘
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

	this->counts = 0;
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
	if (this->ui.enhanceCheckBox->isChecked()) {
		counts++;
		this->mode = 4;
	}
	// TODO: 输入错误处理 或 
	//if (counts != 1) {
	//	QMessageBox::critical()
	//}
}

void ImageProcessing::startProcessing() {
	string path_temp;
	Img img_temp;

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
	case 4:
		img_temp = tools.EnhanceDetails(this->img);
		img_temp.SaveImg("data/temp/4." + this->img.get_img_type());
	case 3:
		if (this->mode == 3) {
			// 注意作用域的问题
			img_temp = tools.GrayLevelWindow(this->img, ui.posLineEdit->text().toInt(), ui.widthLineEdit->text().toInt());
			img_temp.SaveImg("data/temp/3." + this->img.get_img_type());
		}
		else if (this->ui.mappingCheckBox->isChecked()) {
			img_temp = tools.GrayLevelWindow(img, ui.posLineEdit->text().toInt(), ui.widthLineEdit->text().toInt());
		}

		CustomInfo img_info = img_temp.get_img_info();
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

	if (this->img.get_img_type() == "bmp") {
		QImage* img_dst = new QImage;
		img_dst->load(QString::fromStdString(path_temp));
		ui.dstLabel->setPixmap(QPixmap::fromImage(*img_dst));
		ui.dstLabel->adjustSize();
		ui.dstScrollArea->setWidget(ui.dstLabel);
	}
}


void ImageProcessing::saveImg() {
	// TODO: 读取自定义地址，根据自定义输出类型对应转换图像类型，保存图像
	string path = this->img.get_file_path() + "_output" + std::to_string(this->counts++) + "." + this->img.get_img_type(), path_temp;

	switch (this->mode) {
	case 1:
		path_temp = "data/temp/1." + this->img.get_img_type();
		rename(path_temp.c_str(), path.c_str());
		break;
	case 2:
		path_temp = "data/temp/2." + this->img.get_img_type();
		rename(path_temp.c_str(), path.c_str());
		break;
	case 3:
		path_temp = "data/temp/3." + this->img.get_img_type();
		rename(path_temp.c_str(), path.c_str());
		break;
	case 4:
		path_temp = "data/temp/4." + this->img.get_img_type();
		rename(path_temp.c_str(), path.c_str());
		break;
	}
}
