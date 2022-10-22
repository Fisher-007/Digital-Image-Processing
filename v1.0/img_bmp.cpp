#include "img_bmp.h"
#include "message.h"
#include <malloc.h>


Bmp::Bmp() {}

Bmp::Bmp(string file, string location):BaseImg(file, location){
}

void Bmp::LoadBmpImg(){
    char* buffer;
    char* p;

    FILE* fp;
    fopen_s(&fp, this->file_path.c_str(), "rb");

    if (fp == NULL) {
        ExitMessage("图片文件打开失败！");
    }

    fread(&this->bmp_info.bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&this->bmp_info.bi, sizeof(BITMAPINFOHEADER), 1, fp);

    if (this->bmp_info.bf.bfType != 'MB') {
        fclose(fp);
        ExitMessage("图片文件类型错误！");
    }

    if (this->bmp_info.bi.biBitCount != 24) {
        fclose(fp);
        ExitMessage("不支持该格式的bmp图片！");
    }

    fseek(fp, this->bmp_info.bf.bfOffBits, 0);

    buffer = (char*)malloc(this->bmp_info.bi.biWidth * this->bmp_info.bi.biHeight * 3);
    fread(buffer, 1, this->bmp_info.bi.biWidth * this->bmp_info.bi.biHeight * 3, fp);

    p = buffer;

    vector<vector<uchar>> img_data;
    vector<uchar> rgb;
    for (int y = 0; y < this->bmp_info.bi.biHeight; y++) {
        for (int x = 0; x < this->bmp_info.bi.biWidth; x++) {

            rgb.clear();
            rgb.push_back(*(p++));
            rgb.push_back(*(p++));
            rgb.push_back(*(p++));

            if (x == this->bmp_info.bi.biWidth - 1)
                for (int k = 0; k < this->bmp_info.bi.biWidth % 4; k++) 
                    p++;

            img_data.push_back(rgb);
        }
    }

    this->bmp_info.img_data = img_data;

    fclose(fp);
}

void Bmp::SaveBmpImg(string save_path) const {
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

    fwrite(&this->bmp_info.bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&this->bmp_info.bi, sizeof(BITMAPINFOHEADER), 1, fp);

    char ch = '0';
    int size = this->bmp_info.bi.biWidth * this->bmp_info.bi.biHeight;
    for (int i = 0; i < size; i++) {
        fwrite(&this->bmp_info.img_data.at(i).at(0), 1, 1, fp);
        fwrite(&this->bmp_info.img_data.at(i).at(1), 1, 1, fp);
        fwrite(&this->bmp_info.img_data.at(i).at(2), 1, 1, fp);

        if (i % this->bmp_info.bi.biWidth == this->bmp_info.bi.biWidth - 1)
            for (int j = 0; j < this->bmp_info.bi.biWidth % 4; j++)
                fwrite(&ch, 1, 1, fp);
    }

    fclose(fp);
    StatusMessage("已保存图像至: " + save_path);
}

void Bmp::DisplayBmpImg(int x_offset, int y_offset) const {
    HWND hWindow = GetForegroundWindow();
    HDC hDc = GetDC(hWindow);
    char blue, green, red;
    int x, y;

    for (int i = 0; i < this->bmp_info.img_data.size(); i++) {
        blue = this->bmp_info.img_data.at(i).at(0);
        green = this->bmp_info.img_data.at(i).at(1);
        red = this->bmp_info.img_data.at(i).at(2);

        x = i % this->bmp_info.bi.biWidth + x_offset;
        y = this->bmp_info.bi.biHeight - i / this->bmp_info.bi.biWidth + y_offset;

        SetPixel(hDc, x, y, RGB(red, green, blue));
    }
}
