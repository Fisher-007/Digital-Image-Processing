#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_gui.h"

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    GUI(QWidget *parent = nullptr);
    ~GUI();

private:
    Ui::GUIClass ui;

private slots:
    void initImg();
};
