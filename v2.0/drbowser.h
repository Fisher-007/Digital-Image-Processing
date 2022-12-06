#pragma once

#include <QMainWindow>
#include <qmessagebox.h>
#include "ui_drbowser.h"
#include "img.h"
#include "processing.h"
#include <string>
using std::string;

class DRBowser : public QMainWindow
{
	Q_OBJECT

public:
	DRBowser(string file, string location, QWidget *parent = nullptr);
	~DRBowser();

private:
	Ui::DRBowserClass ui;
	Img img;
	Processing tools;
	int proc_counts, save_counts;

private slots:
	void startProcessing();
	void saveImg();
};
