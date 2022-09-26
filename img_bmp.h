#pragma once
#ifndef IMG_BMP_H
#define IMG_BMP_H

#include "base_img.h"
#include <Windows.h>
#include <vector>
using std::vector;


//自定义了一个ImgInfo的结构体，包含BMP文件头、BMP信息头和像素点的RGB值。
//目前只支持24位图像的读取和显示
typedef struct {
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	vector<vector<char>> imgData;
}BmpInfo;


class Bmp :virtual public BaseImg {

protected:
	BmpInfo bmp_info;
public:
	Bmp();
	Bmp(string file, string location = "data/");
	void LoadBmpImg();
	void SaveBmpImg(string save_path = "") const;
	void DisplayBmpImg() const;
};

#endif