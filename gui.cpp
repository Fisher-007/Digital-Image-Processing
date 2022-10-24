// #include "stdafx.h"
#include <QtWidgets>
#include "gui.h"
#include "image_processing.h"

GUI::GUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

GUI::~GUI()
{}

void GUI::initImg() {
    ImageProcessing *w = new ImageProcessing(ui.fileLineEdit->text().toStdString(), ui.locationLineEdit->text().toStdString(), this);
    w->show();
}