#pragma once
#ifndef IMG_H
#define IMG_H

#include "img_bmp.h"
#include "img_custom.h"
#include <string>
#include <map>
using std::string;
using std::map;


enum ImgType { BMP, JPG, PNG, CUSTOM};
extern map<string, int> img_type_set;


// Img类继承自所有常见的图片格式类，可继续进行拓展
class Img :public Bmp, public Custom {

private:
	string img_type;
	void set_img_type(string file);
	void ConvertCustomToBMP();

public:
	Img();
	Img(string file, string location = "data/");
	string get_img_type() const;
	CustomInfo get_img_info() const;
	void LoadImg();
	void DisplayImg(int x_offset = 0, int y_offset = 120) const;
	void SaveImg(string save_path = "") const;
	void NewImgInfo(Img& output, vector<vector<uchar>> img_data, int width = -1, int height = -1) const;
	void NewImgInfo(Img& output, vector<unsigned short> img_data, int width = -1, int height = -1) const;
	void ConvertImgType(string to_type);
	friend class Processing;
};

#endif