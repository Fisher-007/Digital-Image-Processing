#pragma once
#ifndef IMG_BMP_H
#define IMG_BMP_H

#include "base_img.h"
#include <Windows.h>
#include <vector>
using std::vector;


typedef struct {
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	vector<vector<uchar>> img_data;
}BmpInfo;


class Bmp :virtual public BaseImg {

protected:
	BmpInfo bmp_info;
	Bmp();
	Bmp(string file, string location = "data/");
	// void set_bmp_info(vector<vector<uchar>> data);
	void LoadBmpImg();
	void SaveBmpImg(string save_path = "") const;
	void DisplayBmpImg(int x_offset = 0, int y_offset = 120) const;
	friend class Processing;
};

#endif