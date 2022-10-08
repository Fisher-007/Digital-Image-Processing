#pragma once
#ifndef PROCESSING_H
#define PROCESSING_H

#include "img.h"
#include <opencv2/opencv.hpp>
using cv::Mat;


class Processing {

private:
	Img ConvertToImg(const Img& img, const Mat& result);

public:
	void DisplayEffect(const Img& img_origin, const Img& img_processed);
	Img FourierTransform(const Img& img);
	Img FourierInverseTransform(const Img& img);
	Img HistogramEqualization(const Img& img);
	class GeometricTransform {
	private:
		//Img BilinearInterpolation();
	public:
		Img Mirror(const Img& img);
		Img Rotation(const Img& img);
		Img Scaling(const Img& img);
	} GeometricTransform;
};


#endif