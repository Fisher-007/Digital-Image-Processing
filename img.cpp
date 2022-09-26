#include "img.h"


map<string, int> img_type_set = { {"bmp", 0}, {"jpg",1}, {"png", 2} };


// ���ڽ���ͼ������̺�õ���ͼ������
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
	// TODO: �쳣����
	this->img_type = file.substr(pos + 1);
}

void Img::LoadImg() {
	switch (img_type_set[this->img_type]) {
	case BMP:
		LoadBmpImg();
	// TODO: ��չ��������
		break;
	}
}

void Img::DisplayImg() const {
	switch (img_type_set[this->img_type]) {
	case BMP:
		DisplayBmpImg();
		// TODO: ��չ��������
		break;
	}
}

void Img::SaveImg(string save_path) const {
	switch (img_type_set[this->img_type]) {
	case BMP:
		SaveBmpImg(save_path);
		// TODO: ��չ��������
		break;
	}
}

//void Img::ConvertImgType() {
//	// TODO: ���������ø�ʽ��ͼƬת��Ϊbmp��ʽ
//}