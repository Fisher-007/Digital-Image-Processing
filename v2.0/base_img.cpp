#include "base_img.h"


BaseImg::BaseImg() {}

BaseImg::BaseImg(string file, string location) {
	this->file_path = location + file;
}

const string BaseImg::get_file_path() const {
	return this->file_path;
}

BaseImg::~BaseImg() {}