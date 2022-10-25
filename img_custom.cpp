#include "img_custom.h"
#include "message.h"
#include <malloc.h>


Custom::Custom() {}

Custom::Custom(string file, string location) :BaseImg(file, location) {
}

// void Custom::set_custom_info() {}

void Custom::LoadCustomImg() {

    FILE* fp;
    fopen_s(&fp, this->file_path.c_str(), "rb");

    if (fp == NULL) {
        ExitMessage("图片文件打开失败！");
    }

    fread(&this->custom_info.width, sizeof(unsigned long), 1, fp);
    fread(&this->custom_info.height, sizeof(unsigned long), 1, fp);

    //unsigned short* buffer;
    //unsigned short* p;

    //buffer = (unsigned short*)malloc(this->custom_info.width * this->custom_info.height);
    //int t;
    //t = fread(buffer, sizeof(unsigned short), this->custom_info.width * this->custom_info.height, fp);
    //int t2 = feof(fp);

    //p = buffer;

    //vector<unsigned short> img_data;
    //for (int y = 0; y < this->custom_info.height; y++) {
    //    for (int x = 0; x < this->custom_info.width; x++) {
    //        img_data.push_back(*p);
    //        if (y * this->custom_info.width + (x + 1) < this->custom_info.width * this->custom_info.height)
    //            p++;
    //    }
    //}

    unsigned short buf;
    vector<unsigned short> img_data;
    while (!feof(fp)) {
        fread(&buf, sizeof(unsigned short), 1, fp);
        if (!feof(fp))
            img_data.push_back(buf);
    }

    if (img_data.size() != this->custom_info.width * this->custom_info.height)
        ExitMessage("文件信息有误！");

    this->custom_info.img_data = img_data;

    fclose(fp);
}

void Custom::SaveCustomImg(string save_path) const {
    if (save_path.empty()) {
        int pos_dot = this->file_path.find_last_of(".");
        int pos_file = this->file_path.find_last_of("/");
        save_path = this->file_path.substr(0, pos_file + 1)
            + this->file_path.substr(pos_file + 1).substr(0, pos_dot)
            + "_processed" + this->file_path.substr(pos_dot);
    }

    FILE* fp;
    fopen_s(&fp, save_path.c_str(), "wb");

    if (fp == NULL) {
        ExitMessage("图片文件打开失败！");
    }

    fwrite(&this->custom_info.width, sizeof(unsigned long), 1, fp);
    fwrite(&this->custom_info.height, sizeof(unsigned long), 1, fp);

    int size = this->custom_info.width * this->custom_info.height;
    for (int i = 0; i < size; i++) {
        fwrite(&this->custom_info.img_data.at(i), sizeof(unsigned short), 1, fp);
    }

    fclose(fp);
    StatusMessage("已保存图像至: " + save_path);
}

void Custom::DisplayCustomImg(int x_offset, int y_offset) const {
	return;
}