#pragma once
#ifndef IMG_CUSTOM_H
#define IMG_CUSTOM_H

#include "base_img.h"
#include <vector>
using std::vector;


typedef struct {
	unsigned long height;
	unsigned long width;
	vector<unsigned short> img_data;
}CustomInfo;


class Custom : virtual public BaseImg {

protected:
	CustomInfo custom_info;
	Custom();
	Custom(string file, string location = "data/");
	// void set_custom_info();
	void LoadCustomImg();
	void SaveCustomImg(string save_path = "") const;
	void DisplayCustomImg(int x_offset = 0, int y_offset = 120) const;
	friend class Processing;
};

#endif