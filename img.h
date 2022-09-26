#pragma once
#ifndef IMG_H
#define IMG_H

#include "img_bmp.h"
#include <string>
#include <map>
using std::string;
using std::map;


enum ImgType { BMP, JPG, PNG };
extern map<string, int> img_type_set;


// Img��̳������г�����ͼƬ��ʽ�࣬�ɼ���������չ
class Img :public Bmp {

private:
	string img_type;
	void set_img_type(string file);
	// void ConvertImgType();

public:
	Img();
	Img(string file, string location = "data/");
	void LoadImg();
	void DisplayImg() const;
	void SaveImg(string save_path = "") const;
};

#endif