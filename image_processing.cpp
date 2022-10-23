#include "image_processing.h"

ImageProcessing::ImageProcessing(string file, string location, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->img = Img(file, location);
}

ImageProcessing::~ImageProcessing()
{}
