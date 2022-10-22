#pragma once
#ifndef PROCESSING_H
#define PROCESSING_H

#include "img.h"
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
		// TODO: 异常处理机制 
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
	public:
		Img Mirror(const Img& img);
		Img Rotation(const Img& img, float angle, int rx = -1, int ry = -1);
		Img Scaling(const Img& img, float multiple);
	} GeometricTransform;

	// TODO: 颜色&&多通道拓展
};


// 椒盐噪声
void saltAndPepper(Mat& image, int n);

// 自适应中值滤波
void AdaptiveLocalNoiseReductionFilter(Mat img_input, Mat& img_output, int m, int n); //输入图像，输出图像，m,n为滤波器大小。

int main2();


#endif