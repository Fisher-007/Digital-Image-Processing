#include "processing.h"
#include "message.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utils/logger.hpp>
using namespace cv;

#define PI 3.1415926


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

vector<uchar> Processing::GeometricTransform::BilinearInterpolation(const Img& img, float x0, float y0) {
	int x1 = floor(x0), x2 = ceil(x0), y1 = floor(y0), y2 = ceil(y0);
	vector<uchar> output(3);

	if (x1 == x0 && x0 == x2 && y1 == y0 && y0 == y2)
		output = img.bmp_info.img_data[y0 * img.bmp_info.bi.biWidth + x0];

	else if (x1 == x0 && x0 == x2)
		for (int i = 0; i < 3; i++) {
			output[i] = img.bmp_info.img_data[y2 * img.bmp_info.bi.biWidth + x0][i] - (y2 - y0) / (y2 - y1)
				* (img.bmp_info.img_data[y2 * img.bmp_info.bi.biWidth + x0][i] - img.bmp_info.img_data[y1 * img.bmp_info.bi.biWidth + x0][i]);
		}

	else if (y1 == y0 && y0 == y2)
		for (int i = 0; i < 3; i++) {
			output[i] = img.bmp_info.img_data[y0 * img.bmp_info.bi.biWidth + x2][i] - (x2 - x0) / (x2 - x1)
				* (img.bmp_info.img_data[y0 * img.bmp_info.bi.biWidth + x2][i] - img.bmp_info.img_data[y0 * img.bmp_info.bi.biWidth + x1][i]);
		}

	else {
		vector<uchar> temp1(3), temp2(3);
		for (int i = 0; i < 3; i++) {
			temp1[i] = img.bmp_info.img_data[y1 * img.bmp_info.bi.biWidth + x2][i] - (x2 - x0) / (x2 - x1)
				* (img.bmp_info.img_data[y1 * img.bmp_info.bi.biWidth + x2][i] - img.bmp_info.img_data[y1 * img.bmp_info.bi.biWidth + x1][i]);
			temp2[i] = img.bmp_info.img_data[y2 * img.bmp_info.bi.biWidth + x2][i] - (x2 - x0) / (x2 - x1)
				* (img.bmp_info.img_data[y2 * img.bmp_info.bi.biWidth + x2][i] - img.bmp_info.img_data[y2 * img.bmp_info.bi.biWidth + x1][i]);
			output[i] = temp2[i] - (y2 - y0) / (y2 - y1) * (temp2[i] - temp1[i]);
		}
	}

	return output;
}

Img Processing::GeometricTransform::Mirror(const Img& img) {

	vector<vector<uchar>> img_data(img.bmp_info.bi.biHeight * img.bmp_info.bi.biWidth);
	for (int i = 0; i < img_data.size(); i++)
		img_data[i].resize(3);

	int x1, y1;
	for (int y = 0; y < img.bmp_info.bi.biHeight; y++) {
		for (int x = 0; x < img.bmp_info.bi.biWidth; x++) {
			x1 = img.bmp_info.bi.biWidth - x - 1;
			y1 = y;
			img_data[y1 * img.bmp_info.bi.biWidth + x1] = img.bmp_info.img_data[y * img.bmp_info.bi.biWidth + x];
		}
	}

	Img output;
	output.bmp_info.img_data = img_data;
	output.bmp_info.bf = img.bmp_info.bf;
	output.bmp_info.bi = img.bmp_info.bi;

	return output;
}

Img Processing::GeometricTransform::Rotation(const Img& img, float angle, int rx, int ry) {

	if (rx == -1)
		rx = round(img.bmp_info.bi.biWidth / 2);
	if (ry == -1)
		ry = round(img.bmp_info.bi.biHeight / 2);

	vector<vector<uchar>> img_data(img.bmp_info.bi.biHeight * img.bmp_info.bi.biWidth);
	for (int i = 0; i < img_data.size(); i++)
		img_data[i].resize(3, 0);

	int x1, y1;
	for (int y = 0; y < img.bmp_info.bi.biHeight; y++) {
		for (int x = 0; x < img.bmp_info.bi.biWidth; x++) {

			x1 = round(cos(angle * PI / 180) * (x - rx) + sin(angle * PI / 180) * (y - ry) + rx);
			y1 = round( - sin(angle * PI / 180) * (x - rx) + cos(angle * PI / 180) * (y - ry) + ry);

			if (x1 >= 0 && x1 < img.bmp_info.bi.biWidth && y1 >= 0 && y1 < img.bmp_info.bi.biHeight)
				img_data[y1 * img.bmp_info.bi.biWidth + x1] = img.bmp_info.img_data[y * img.bmp_info.bi.biWidth + x];
		}
	}

	float x0, y0;
	for (int y = 0; y < img.bmp_info.bi.biHeight; y++) {
		for (int x = 0; x < img.bmp_info.bi.biWidth; x++) {

			x0 = cos(angle * PI / 180) * (x - rx) - sin(angle * PI / 180) * (y - ry) + rx;
			y0 = sin(angle * PI / 180) * (x - rx) + cos(angle * PI / 180) * (y - ry) + ry;

			if (x0 >= 0 && x0 <= img.bmp_info.bi.biWidth - 1 && y0 >= 0 && y0 <= img.bmp_info.bi.biHeight - 1)
				if (img_data[y * img.bmp_info.bi.biWidth + x][0] == img_data[y * img.bmp_info.bi.biWidth + x][1] == img_data[y * img.bmp_info.bi.biWidth + x][2] == 0)
					img_data[y * img.bmp_info.bi.biWidth + x] = BilinearInterpolation(img, x0, y0);
		}
	}

	Img output;
	output.bmp_info.img_data = img_data;
	output.bmp_info.bf = img.bmp_info.bf;
	output.bmp_info.bi = img.bmp_info.bi;

	return output;
}

Img Processing::GeometricTransform::Scaling(const Img& img, float multiple) {

	int width, height;
	width = round(img.bmp_info.bi.biWidth * multiple);
	height = round(img.bmp_info.bi.biHeight * multiple);

	vector<vector<uchar>> img_data(width * height);
	for (int i = 0; i < img_data.size(); i++)
		img_data[i].resize(3, 0);

	int x1, y1;
	for (int y = 0; y < img.bmp_info.bi.biHeight; y++) {

		if (y == img.bmp_info.bi.biHeight - 1)
			y1 = height - 1;
		else
			y1 = round(multiple * y);

		for (int x = 0; x < img.bmp_info.bi.biWidth; x++) {

			if (x == img.bmp_info.bi.biWidth - 1)
				x1 = width - 1;
			else
				x1 = round(multiple * x);

			img_data[y1 * width + x1] = img.bmp_info.img_data[y * img.bmp_info.bi.biWidth + x];
		}
	}

	if (multiple > 1) {
		float x0, y0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (img_data[y * width + x][0] == img_data[y * width + x][1] == img_data[y * width + x][2] == 0) {

					if (y == height - 1)
						y0 = img.bmp_info.bi.biHeight - 1;
					else
						y0 = y / multiple;

					if (x == width - 1)
						x0 = img.bmp_info.bi.biWidth - 1;
					else
						x0 = x / multiple;

					img_data[y * width + x] = BilinearInterpolation(img, x0, y0);
				}
			}
		}
	}

	Img output;
	output.bmp_info.img_data = img_data;
	output.bmp_info.bf = img.bmp_info.bf;
	output.bmp_info.bi = img.bmp_info.bi;
	output.bmp_info.bi.biWidth = width;
	output.bmp_info.bi.biHeight = height;

	return output;
}