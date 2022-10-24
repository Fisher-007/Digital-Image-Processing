#pragma once

#include <QMainWindow>
#include <qmessagebox.h>
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
	int mode;
	void set_mode();

private slots:
	void startProcessing();
};
