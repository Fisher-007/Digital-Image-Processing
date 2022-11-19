#include "drbowser.h"

DRBowser::DRBowser(string file, string location, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->img = Img(file, location);

	if (img.get_img_type() == "raw") {
		CustomInfo img_info = this->img.get_img_info();
		QImage* img_origin = new QImage(img_info.width, img_info.height, QImage::Format_Grayscale8);
		for (int y = 0; y < img_info.height; y++) {
			for (int x = 0; x < img_info.width; x++) {
				int value = img_info.img_data.at(y * img_info.width + x);
				img_origin->setPixel(x, y, qRgb(value, value, value));
			}
		}
		ui.imgDisplayLabel->setPixmap(QPixmap::fromImage(*img_origin));
		ui.imgDisplayLabel->adjustSize();
		ui.scrollArea->setWidget(ui.imgDisplayLabel);
	}
	else {
		QMessageBox::critical(this, "Error", "Img type error!");
		this->setAttribute(Qt::WA_DeleteOnClose, true);
		this->close();
	}

	proc_counts = save_counts = 0;
	system("md data\\temp");
}


DRBowser::~DRBowser() {
	system("rd /s /q data\\temp");
}


void DRBowser::startProcessing() {
	Img img_temp = img;

	if (this->ui.scalingCheckBox->isChecked()) {
		img_temp = tools.GeometricTransform.Scaling(img_temp, ui.multipleLineEdit->text().toFloat());
	}
	if (this->ui.mirrorCheckBox->isChecked()) {
		img_temp = tools.GeometricTransform.Mirror(img_temp);
	}
	if (this->ui.enhanceCheckBox->isChecked()) {
		img_temp = tools.EnhanceDetails(img_temp);
	}

	if (this->ui.glwCheckBox->isChecked()) {
		img_temp = tools.GrayLevelWindow(img_temp, ui.posLineEdit->text().toInt(), ui.widthLineEdit->text().toInt());
	}
	if (this->ui.revesalCheckBox->isChecked()) {
		img_temp = tools.GrayLevelRevesal(img_temp);
	}

	img_temp.SaveImg("data/temp/output" + std::to_string(this->proc_counts++) + "." + this->img.get_img_type());

	CustomInfo img_info = img_temp.get_img_info();
	QImage* img_dst = new QImage(img_info.width, img_info.height, QImage::Format_Grayscale8);
	for (int y = 0; y < img_info.height; y++) {
		for (int x = 0; x < img_info.width; x++) {
			int value = img_info.img_data.at(y * img_info.width + x);
			img_dst->setPixel(x, y, qRgb(value, value, value));
		}
	}
	ui.imgDisplayLabel->setPixmap(QPixmap::fromImage(*img_dst));
	ui.imgDisplayLabel->adjustSize();
	ui.scrollArea->setWidget(ui.imgDisplayLabel);
}


void DRBowser::saveImg() {
	string path, path_temp;

	if (this->ui.pathLineEdit->text().isEmpty())
		path = "data/output" + std::to_string(this->save_counts++) + "." + this->img.get_img_type();
	else
		path = this->ui.pathLineEdit->text().toStdString();

	path_temp = "data/temp/output" + std::to_string(this->proc_counts - 1) + "." + this->img.get_img_type();

	if (this->img.get_img_type() == path.substr(path.find_last_of(".") + 1))
		rename(path_temp.c_str(), path.c_str());
	else {
		Img img_temp("temp/output" + std::to_string(this->proc_counts - 1) + "." + this->img.get_img_type());
		img_temp.ConvertImgType("bmp");
		img_temp.SaveImg(path);
	}
	// TODO: 没有变化的情况下连续按无效？
}
