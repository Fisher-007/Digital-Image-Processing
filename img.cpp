#include "img.h"


map<string, int> img_type_set = { {"bmp", 0}, {"jpg",1}, {"png", 2} };


// 用于接收图像处理过程后得到的图像数据
Img::Img() {}

Img::Img(string file, string location) {
	this->file_path = location + file;
	set_img_type(file);
	if (img_type_set.count(this->img_type)) {
		// TODO: raise error!
	}
	LoadImg();
}

void Img::set_img_type(string file) {
	int pos = file.find_last_of(".");
	// TODO: 异常处理
	this->img_type = file.substr(pos + 1);
}

void Img::LoadImg() {
	switch (img_type_set[this->img_type]) {
	case BMP:
		LoadBmpImg();
	// TODO: 拓展其他类型
		break;
	}
}

void Img::DisplayImg(int x_offset, int y_offset) const {
	switch (img_type_set[this->img_type]) {
	case BMP:
		DisplayBmpImg(x_offset, y_offset);
		// TODO: 拓展其他类型
		break;
	}
}

void Img::SaveImg(string save_path) const {
	switch (img_type_set[this->img_type]) {
	case BMP:
		SaveBmpImg(save_path);
		// TODO: 拓展其他类型
		break;
	}
}

//void Img::ConvertImgType() {
//	// TODO: 将其他常用格式的图片转换为bmp格式
//}