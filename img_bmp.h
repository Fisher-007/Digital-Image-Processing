#pragma once
#ifndef IMG_BMP_H
#define IMG_BMP_H

#include "base_img.h"
#include <Windows.h>
#include <vector>
using std::vector;


//�Զ�����һ��ImgInfo�Ľṹ�壬����BMP�ļ�ͷ��BMP��Ϣͷ�����ص��RGBֵ��
//Ŀǰֻ֧��24λͼ��Ķ�ȡ����ʾ
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