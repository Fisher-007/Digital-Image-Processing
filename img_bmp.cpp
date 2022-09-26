#include "img_bmp.h"
#include <malloc.h>
#include <iostream>
using namespace std;


Bmp::Bmp() {}

Bmp::Bmp(string file, string location):BaseImg(file, location){
}

void Bmp::LoadBmpImg(){
    char* buf;
    char* p;

    FILE* fp;
    fopen_s(&fp, this->file_path.c_str(), "rb");
    if (fp == NULL) {
        cout << "打开文件失败!" << endl;
        exit(0);
    }

    fread(&this->bmp_info.bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&this->bmp_info.bi, sizeof(BITMAPINFOHEADER), 1, fp);

    if (this->bmp_info.bi.biBitCount != 24) {
        cout << this->bmp_info.bi.biBitCount << endl;
        cout << "不支持该格式的BMP位图！" << endl;
        exit(0);
    }

    fseek(fp, this->bmp_info.bf.bfOffBits, 0);

    buf = (char*)malloc(this->bmp_info.bi.biWidth * this->bmp_info.bi.biHeight * 3);
    fread(buf, 1, this->bmp_info.bi.biWidth * this->bmp_info.bi.biHeight * 3, fp);

    p = buf;

    vector<vector<char>> imgData;
    for (int y = 0; y < this->bmp_info.bi.biHeight; y++) {
        for (int x = 0; x < this->bmp_info.bi.biWidth; x++) {
            vector<char> vRGB;

            vRGB.push_back(*(p++));     //blue
            vRGB.push_back(*(p++));     //green
            vRGB.push_back(*(p++));     //red

            if (x == this->bmp_info.bi.biWidth - 1)
            {
                for (int k = 0; k < this->bmp_info.bi.biWidth % 4; k++) p++;
            }
            imgData.push_back(vRGB);
        }
    }
    fclose(fp);
    this->bmp_info.imgData = imgData;
}

void Bmp::SaveBmpImg(string save_path) const {
    if (save_path.empty()) {
        int pos_dot = this->file_path.find_last_of(".");
        int pos_file = this->file_path.find_last_of("/");
        save_path = this->file_path.substr(0, pos_file + 1) 
            + this->file_path.substr(pos_file + 1).substr(0, pos_dot)
            + this->file_path.substr(pos_dot);
    }
    FILE* fpw;
    fopen_s(&fpw, save_path.c_str(), "wb");
    fwrite(&this->bmp_info.bf, sizeof(BITMAPFILEHEADER), 1, fpw);  //写入文件头
    fwrite(&this->bmp_info.bi, sizeof(BITMAPINFOHEADER), 1, fpw);  //写入文件头信息

    int size = this->bmp_info.bi.biWidth * this->bmp_info.bi.biHeight;
    for (int i = 0; i < size; i++) {
        fwrite(&this->bmp_info.imgData.at(i).at(0), 1, 1, fpw);
        fwrite(&this->bmp_info.imgData.at(i).at(1), 1, 1, fpw);
        fwrite(&this->bmp_info.imgData.at(i).at(2), 1, 1, fpw);

        if (i % this->bmp_info.bi.biWidth == this->bmp_info.bi.biWidth - 1) {
            char ch = '0';
            for (int j = 0; j < this->bmp_info.bi.biWidth % 4; j++) {
                fwrite(&ch, 1, 1, fpw);
            }
        }
    }
    fclose(fpw);
    cout << "已保存图像至: " + save_path << endl;
}

void Bmp::DisplayBmpImg() const {
    HWND hWindow;                                                //窗口句柄
    HDC hDc;                                                     //绘图设备环境句柄
    int yOffset = 150;
    hWindow = GetForegroundWindow();
    hDc = GetDC(hWindow);

    int posX, posY;
    for (int i = 0; i < this->bmp_info.imgData.size(); i++) {
        char blue = this->bmp_info.imgData.at(i).at(0);
        char green = this->bmp_info.imgData.at(i).at(1);
        char red = this->bmp_info.imgData.at(i).at(2);

        posX = i % this->bmp_info.bi.biWidth;
        posY = this->bmp_info.bi.biHeight - i / this->bmp_info.bi.biWidth + yOffset;
        SetPixel(hDc, posX, posY, RGB(red, green, blue));
    }
}
