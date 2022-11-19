// #include "stdafx.h"
#include <QtWidgets>
#include "gui.h"
#include "drbowser.h"
#include "image_processing.h"

GUI::GUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

GUI::~GUI()
{}

void GUI::initImg() {
    
    if (ui.bowserCheckBox->isChecked()) {
        DRBowser* w = new DRBowser(ui.fileLineEdit->text().toStdString(), ui.locationLineEdit->text().toStdString(), this);
        w->show();
    }
    else {
        ImageProcessing* w = new ImageProcessing(ui.fileLineEdit->text().toStdString(), ui.locationLineEdit->text().toStdString(), this);
        w->show();
    }
    // w->showFullScreen();
}