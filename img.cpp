#include "img.h"


map<string, int> img_type_set = { {"bmp", 0}, {"jpg",1}, {"png", 2}, {"raw", 3}};


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

string Img::get_img_type() const {
	return this->img_type;
}

CustomInfo Img::get_img_info() const {
	//if (this->img_type == "bmp")
	//	return this->bmp_info;
	//if (this->img_type == "raw")
		return this->custom_info;
}


// TODO: 拓展其他类型
void Img::NewImgInfo(Img & output, vector<vector<uchar>> img_data, int width, int height) const {

	output.bmp_info.img_data = img_data;
	output.bmp_info.bf = this->bmp_info.bf;
	output.bmp_info.bi = this->bmp_info.bi;
	if (width != -1)
		output.bmp_info.bi.biWidth = width;
	if (height != -1)
		output.bmp_info.bi.biHeight = height;
}

void Img::NewImgInfo(Img& output, vector<unsigned short> img_data, int width, int height) const {

	output.custom_info.img_data = img_data;
	output.custom_info.width = this->custom_info.width;
	output.custom_info.height = this->custom_info.height;
	if (width != -1)
		output.custom_info.width = width;
	if (height != -1)
		output.custom_info.height = height;

}

void Img::LoadImg() {
	switch (img_type_set[this->img_type]) {
	case BMP:
		LoadBmpImg();
		break;
	case CUSTOM:
		LoadCustomImg();
		break;
	}
}

void Img::DisplayImg(int x_offset, int y_offset) const {
	switch (img_type_set[this->img_type]) {
	case BMP:
		DisplayBmpImg(x_offset, y_offset);
		break;
	case CUSTOM:
		DisplayCustomImg(x_offset, y_offset);
		break;
	}
}

void Img::SaveImg(string save_path) const {
	switch (img_type_set[this->img_type]) {
	case BMP:
		SaveBmpImg(save_path);
		break;
	case CUSTOM:
		SaveCustomImg(save_path);
		break;
	}
}

//void Img::ConvertImgType() {
//	// TODO: 将其他常用格式的图片转换为bmp格式
//}