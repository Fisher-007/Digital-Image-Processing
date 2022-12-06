#pragma once
// #pragma execution_character_set("utf-8")
#ifndef PROCESSING_H
#define PROCESSING_H

#include "img.h"
#include <limits>
#include <opencv2/opencv.hpp>
using cv::Mat;


class Processing {

private:
	static Img ConvertToImg(const Img& img, const Mat& result);

public:
	void DisplayEffect(const Img& img_origin, const Img& img_processed);

	class Fourier {
	private:
		int dm;
		int dn;
		Mat mResult;
		Mat spectrogram;
		void FourierTransform(Mat& img_origin, int mode);
		void FourierInverseTransform(Mat& result);
		// TODO: �쳣�������� 
	public:
		Img FourierTransform(const Img& img);
		Img FourierInverseTransform(const Img& img);
		friend class Processing;
	} Fourier;

	Img HistogramEqualization(const Img& img);
	Img HomomorphicFilter(const Img& img);

	class GeometricTransform {
	private:
		vector<uchar> BilinearInterpolation(const Img& img, float x0, float y0);
		unsigned short BilinearInterpolation(const Img& img, float x0, float y0, int tag);
	public:
		Img Mirror(const Img& img);
		Img Rotation(const Img& img, float angle, int rx = -1, int ry = -1);
		Img Scaling(const Img& img, float multiple);
	} GeometricTransform;

	Img GrayLevelRevesal(const Img& img);
	Img GrayLevelWindow(const Img& img, int pos, int width);
	Img EnhanceDetails(const Img& img);

	// TODO: ��ɫ&&��ͨ����չ
};


// ��������
void saltAndPepper(Mat& image, int n);

// ����Ӧ��ֵ�˲�
void AdaptiveLocalNoiseReductionFilter(Mat img_input, Mat& img_output, int m, int n); //����ͼ�����ͼ��m,nΪ�˲�����С��

int main2();


#endif