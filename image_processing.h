#pragma once

#include <QMainWindow>
#include "ui_image_processing.h"
#include "img.h"
#include "processing.h"
#include <string>
using std::string;

class ImageProcessing : public QMainWindow
{
	Q_OBJECT

public:
	ImageProcessing(string file, string location, QWidget *parent = nullptr);
	~ImageProcessing();

private:
	Ui::ImageProcessingClass ui;
	Img img;
	Processing tools;
	float angle, multiple;
	int rx, ry, mode, state;
};
