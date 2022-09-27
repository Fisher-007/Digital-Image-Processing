#include "processing.h"
#include "message.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utils/logger.hpp>
using namespace cv;


Img Processing::ConvertToImg(const Img& img, const Mat& result) {
	Img output;
	vector<uchar> sub_img;
	vector<vector<uchar>> data;

	for (int y = result.rows - 1; y >= 0; y--)
		for (int x = 0; x < result.cols; x++) {

			sub_img.clear();
			sub_img.push_back(result.at<uchar>(y, x));
			sub_img.push_back(result.at<uchar>(y, x));
			sub_img.push_back(result.at<uchar>(y, x));

			data.push_back(sub_img);
		}

	output.bmp_info.img_data = data;
	output.bmp_info.bf = img.bmp_info.bf;
	output.bmp_info.bi = img.bmp_info.bi;

	return output;
}


Img Processing::FourierTransform(const Img& img) {
	
	setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	Mat img_origin = imread(img.get_file_path(), 0);
	if (img_origin.data == 0) {
		ExitMessage("图像文件读取失败！");
	}

	// Extending image
	int m = getOptimalDFTSize(img_origin.rows);
	int n = getOptimalDFTSize(img_origin.cols);
	copyMakeBorder(img_origin, img_origin, 0, m - img_origin.rows, 0, n - img_origin.cols, BORDER_CONSTANT, Scalar(0));

	// Fourier transform
	Mat mFourier(img_origin.rows + m, img_origin.cols + n, CV_32FC2, Scalar(0, 0));
	Mat mForFourier[] = { Mat_<float>(img_origin), Mat::zeros(img_origin.size(), CV_32F) };
	Mat mSrc;
	merge(mForFourier, 2, mSrc);
	dft(mSrc, mFourier);

	//channels[0] is the real part of Fourier transform,channels[1] is the imaginary part of Fourier transform 
	vector<Mat> channels;
	split(mFourier, channels);
	Mat mRe = channels[0];
	Mat mIm = channels[1];

	//Calculate the amplitude
	Mat mAmplitude;
	magnitude(mRe, mIm, mAmplitude);

	//Logarithmic scale
	mAmplitude += Scalar(1);
	log(mAmplitude, mAmplitude);

	//The normalized
	normalize(mAmplitude, mAmplitude, 0, 255, NORM_MINMAX);

	Mat mResult(img_origin.rows, img_origin.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < img_origin.rows; i++)
	{
		uchar* pResult = mResult.ptr<uchar>(i);
		float* pAmplitude = mAmplitude.ptr<float>(i);
		for (int j = 0; j < img_origin.cols; j++)
		{
			pResult[j] = (uchar)pAmplitude[j];
		}
	}

	Mat mQuadrant1 = mResult(Rect(mResult.cols / 2, 0, mResult.cols / 2, mResult.rows / 2));
	Mat mQuadrant2 = mResult(Rect(0, 0, mResult.cols / 2, mResult.rows / 2));
	Mat mQuadrant3 = mResult(Rect(0, mResult.rows / 2, mResult.cols / 2, mResult.rows / 2));
	Mat mQuadrant4 = mResult(Rect(mResult.cols / 2, mResult.rows / 2, mResult.cols / 2, mResult.rows / 2));

	Mat mChange1 = mQuadrant1.clone();
	//mQuadrant1 = mQuadrant3.clone();
	//mQuadrant3 = mChange1.clone();
	mQuadrant3.copyTo(mQuadrant1);
	mChange1.copyTo(mQuadrant3);

	Mat mChange2 = mQuadrant2.clone();
	//mQuadrant2 = mQuadrant4.clone();
	//mQuadrant4 = mChange2.clone();
	mQuadrant4.copyTo(mQuadrant2);
	mChange2.copyTo(mQuadrant4);

	namedWindow("Origin Img", WINDOW_NORMAL);
	imshow("Origin Img", img_origin);

	namedWindow("The Fourier transform", WINDOW_NORMAL);
	imshow("The Fourier transform", mResult);

	waitKey();
	destroyAllWindows();

	return img;
}

Img Processing::FourierInverseTransform(const Img& img) {
	return img;
}

Img Processing::HistogramEqualization(const Img& img) {

	setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	// 目前只支持BMP文件
	if (img.img_type != "bmp")
		ExitMessage("目前不支持该图像格式！");

	Mat img_origin = imread(img.file_path);
	if (img_origin.empty()) {
		ExitMessage("图像读取失败");
	}

	Mat gray, heq;
	cvtColor(img_origin, gray, COLOR_BGR2GRAY);

	equalizeHist(gray, heq);

	return ConvertToImg(img, heq);
}

void Processing::DisplayEffect(const Img& img_origin, const Img& img_processed) {
	img_origin.DisplayImg(0, 120);
	img_processed.DisplayImg(img_origin.bmp_info.bi.biWidth + 10, 120);
}