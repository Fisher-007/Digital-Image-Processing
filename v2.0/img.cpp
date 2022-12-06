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
	output.img_type = this->img_type;
}

void Img::NewImgInfo(Img& output, vector<unsigned short> img_data, int width, int height) const {

	output.custom_info.img_data = img_data;
	output.custom_info.width = this->custom_info.width;
	output.custom_info.height = this->custom_info.height;
	if (width != -1)
		output.custom_info.width = width;
	if (height != -1)
		output.custom_info.height = height;
	output.img_type = this->img_type;
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

void Img::ConvertCustomToBMP() {
	// TODO: bmp格式其他位深度的处理/调色板/补齐问题

	this->bmp_info.bf.bfType = ((WORD)('M' << 8) | 'B');
	this->bmp_info.bf.bfReserved1 = 0;
	this->bmp_info.bf.bfReserved2 = 0;
	this->bmp_info.bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) 
		+ this->custom_info.width * this->custom_info.height * 3;
	this->bmp_info.bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	this->bmp_info.bi.biSize = sizeof(BITMAPINFOHEADER);
	this->bmp_info.bi.biWidth = this->custom_info.width;
	this->bmp_info.bi.biHeight = this->custom_info.height;
	this->bmp_info.bi.biSizeImage = this->custom_info.width * this->custom_info.height * 3;
	this->bmp_info.bi.biCompression = BI_RGB;
	this->bmp_info.bi.biPlanes = 1;
	this->bmp_info.bi.biBitCount = 24;
	this->bmp_info.bi.biXPelsPerMeter = 0;
	this->bmp_info.bi.biYPelsPerMeter = 0;
	this->bmp_info.bi.biClrUsed = 256;
	this->bmp_info.bi.biClrImportant = 0;

	vector<vector<uchar>> img_data;
	vector<uchar> rgb;
	for (int y = this->bmp_info.bi.biHeight - 1; y >= 0; y--) {
		for (int x = 0; x < this->bmp_info.bi.biWidth; x++) {

			rgb.clear();
			rgb.push_back((uchar)this->custom_info.img_data[y * this->bmp_info.bi.biWidth + x]);
			rgb.push_back((uchar)this->custom_info.img_data[y * this->bmp_info.bi.biWidth + x]);
			rgb.push_back((uchar)this->custom_info.img_data[y * this->bmp_info.bi.biWidth + x]);

			img_data.push_back(rgb);
		}
	}

	this->bmp_info.img_data = img_data;

	this->img_type = "bmp";
}

void Img::ConvertImgType(string to_type) {
	switch (img_type_set[this->img_type]) {
	case BMP:
		// BMP格式向其他图像格式转换
		break;
	case CUSTOM:
		if (to_type == "bmp")
			ConvertCustomToBMP();
		break;
	}
}