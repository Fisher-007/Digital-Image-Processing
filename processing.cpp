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

Img Processing::Fourier::FourierTransform(const Img& img) {
	
	setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	Mat img_origin = imread(img.get_file_path(), 0);
	if (img_origin.data == 0) {
		ExitMessage("图像文件读取失败！");
	}

	// Extending image
	dm = getOptimalDFTSize(img_origin.rows) - img_origin.rows;
	dn = getOptimalDFTSize(img_origin.cols) - img_origin.cols;
	copyMakeBorder(img_origin, img_origin, 0, dm, 0, dn, BORDER_CONSTANT, Scalar(0));

	// Fourier transform
	Mat mForFourier[] = { Mat_<float>(img_origin), Mat::zeros(img_origin.size(), CV_32F) };
	merge(mForFourier, 2, this->mResult);
	dft(this->mResult, this->mResult);
	// cout << typeid(*mResult.data).name() << endl;
	// cout << this->mResult.type() << endl;

	//channels[0] is the real part of Fourier transform,channels[1] is the imaginary part of Fourier transform 
	vector<Mat> channels;
	split(this->mResult, channels);
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

	this->spectrogram =  Mat(img_origin.rows, img_origin.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < img_origin.rows; i++)
	{
		uchar* pResult = this->spectrogram.ptr<uchar>(i);
		float* pAmplitude = mAmplitude.ptr<float>(i);
		for (int j = 0; j < img_origin.cols; j++)
		{
			pResult[j] = (uchar)pAmplitude[j];
		}
	}

	this->spectrogram = this->spectrogram(Rect(0, 0, this->spectrogram.cols - dn, this->spectrogram.rows - dm));

	Mat mQuadrant1 = this->spectrogram(Rect(this->spectrogram.cols / 2, 0, this->spectrogram.cols / 2, this->spectrogram.rows / 2));
	Mat mQuadrant2 = this->spectrogram(Rect(0, 0, this->spectrogram.cols / 2, this->spectrogram.rows / 2));
	Mat mQuadrant3 = this->spectrogram(Rect(0, this->spectrogram.rows / 2, this->spectrogram.cols / 2, this->spectrogram.rows / 2));
	Mat mQuadrant4 = this->spectrogram(Rect(this->spectrogram.cols / 2, this->spectrogram.rows / 2, this->spectrogram.cols / 2, this->spectrogram.rows / 2));

	Mat mChange1 = mQuadrant1.clone();
	mQuadrant3.copyTo(mQuadrant1);
	mChange1.copyTo(mQuadrant3);
	Mat mChange2 = mQuadrant2.clone();
	mQuadrant4.copyTo(mQuadrant2);
	mChange2.copyTo(mQuadrant4);

	return ConvertToImg(img, this->spectrogram);
}

Img Processing::Fourier::FourierInverseTransform(const Img& img) {
	// TODO: 处理输入的频域图

	Mat result;

	idft(this->mResult, result, DFT_REAL_OUTPUT);
	normalize(result, result, 0, 1, NORM_MINMAX); // TODO: 无法100%还原？&& 32F颜色显示问题？

	result.convertTo(result, CV_8U, 255.0);
	result = result(Rect(0, 0, result.cols - dn, result.rows - dm));

	return ConvertToImg(img, result);
}

void Processing::Fourier::FourierTransform(Mat& img_origin, int mode) {

	setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	// Extending image
	dm = getOptimalDFTSize(img_origin.rows) - img_origin.rows;
	dn = getOptimalDFTSize(img_origin.cols) - img_origin.cols;
	copyMakeBorder(img_origin, img_origin, 0, dm, 0, dn, BORDER_CONSTANT, Scalar(0));

	// Fourier transform
	Mat mForFourier[] = { Mat_<float>(img_origin), Mat::zeros(img_origin.size(), CV_32F) };
	merge(mForFourier, 2, this->mResult);
	dft(this->mResult, this->mResult);

	//channels[0] is the real part of Fourier transform,channels[1] is the imaginary part of Fourier transform 
	vector<Mat> channels;
	split(this->mResult, channels);
	Mat mRe = channels[0];
	Mat mIm = channels[1];

	//Calculate the amplitude
	Mat mAmplitude;
	magnitude(mRe, mIm, mAmplitude);

	//Logarithmic scale
	log(mAmplitude + 1, mAmplitude);

	//The normalized
	normalize(mAmplitude, mAmplitude, 0, 255, NORM_MINMAX);

	this->spectrogram = Mat(img_origin.rows, img_origin.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < img_origin.rows; i++)
	{
		uchar* pResult = this->spectrogram.ptr<uchar>(i);
		float* pAmplitude = mAmplitude.ptr<float>(i);
		for (int j = 0; j < img_origin.cols; j++)
		{
			pResult[j] = (uchar)pAmplitude[j];
		}
	}

	this->spectrogram = this->spectrogram(Rect(0, 0, this->spectrogram.cols - dn, this->spectrogram.rows - dm));

	int cx = this->spectrogram.cols / 2;
	int cy = this->spectrogram.rows / 2;

	Mat mQuadrant1 = this->spectrogram(Rect(cx, 0, cx, cy));
	Mat mQuadrant2 = this->spectrogram(Rect(0, 0, cx, cy));
	Mat mQuadrant3 = this->spectrogram(Rect(0, cy, cx, cy));
	Mat mQuadrant4 = this->spectrogram(Rect(cx, cy, cx, cy));

	Mat mChange1 = mQuadrant1.clone();
	mQuadrant3.copyTo(mQuadrant1);
	mChange1.copyTo(mQuadrant3);
	Mat mChange2 = mQuadrant2.clone();
	mQuadrant4.copyTo(mQuadrant2);
	mChange2.copyTo(mQuadrant4);

	//if (mode == 1) {

	//	Mat tmp;

	//	Mat q00(mRe, Rect(0, 0, cx, cy));   // 左上区域
	//	Mat q01(mRe, Rect(cx, 0, cx, cy));  // 右上区域
	//	Mat q02(mRe, Rect(0, cy, cx, cy));  // 左下区域
	//	Mat q03(mRe, Rect(cx, cy, cx, cy)); // 右下区域
	//	q00.copyTo(tmp); q03.copyTo(q00); tmp.copyTo(q03);//左上与右下进行交换
	//	q01.copyTo(tmp); q02.copyTo(q01); tmp.copyTo(q02);//右上与左下进行交换

	//	Mat q10(mIm, Rect(0, 0, cx, cy));   // 左上区域
	//	Mat q11(mIm, Rect(cx, 0, cx, cy));  // 右上区域
	//	Mat q12(mIm, Rect(0, cy, cx, cy));  // 左下区域
	//	Mat q13(mIm, Rect(cx, cy, cx, cy)); // 右下区域
	//	q10.copyTo(tmp); q13.copyTo(q10); tmp.copyTo(q13);//左上与右下进行交换
	//	q11.copyTo(tmp); q12.copyTo(q11); tmp.copyTo(q12);//右上与左下进行交换

	//	imshow("R1", mRe);
	//	imshow("I1", mIm);

	//	mForFourier[0] = mRe;
	//	mForFourier[1] = mIm;
	//	merge(mForFourier, 2, this->mResult);//将傅里叶变换结果中心化
	//}
}

void Processing::Fourier::FourierInverseTransform(Mat& result) {
	// TODO: 处理输入的频域图

	idft(this->mResult, result, DFT_REAL_OUTPUT);
	result = result(Rect(0, 0, result.cols - dn, result.rows - dm));
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

Mat calc_Homomorphic(Size size, float gamma_L, float gamma_H, float c, float D0) {
	Mat result(size, CV_32FC1);

	int cx = size.width / 2;
	int cy = size.height / 2;

	for (int i = 0; i < result.rows; ++i)
	{
		float* p = result.ptr<float>(i);
		for (int j = 0; j < result.cols; ++j)
		{
			float d_2 = std::pow(i - cy, 2) + std::pow(j - cx, 2);
			//同态滤波传递函数
			p[j] = (gamma_H - gamma_L) * (1 - std::exp(-c * d_2 / (D0 * D0))) + gamma_L;
		}
	}

	//创建双通道图像，便于与复数图像相乘
	Mat planes[] = { result.clone(), result.clone() };
	Mat Homomorphic;
	merge(planes, 2, Homomorphic);

	return Homomorphic;
}

Img Processing::HomomorphicFilter(const Img& img) {

	setLogLevel(utils::logging::LOG_LEVEL_SILENT);
	
	// Mat img_origin = imread(img.file_path);
	Mat img_origin = imread(img.file_path, 0);
	if (img_origin.empty()) {
		ExitMessage("图像读取失败");
	}

	Mat img_temp;
	img_origin.convertTo(img_temp, CV_32F, 1 / 255.0);

	// cvtColor(image, image_gray, COLOR_BGR2GRAY); //转换为灰度图

	// 进行ln变换
	log(img_temp + 1, img_temp); // 为什么必须要加 1

	// 傅里叶变换
	this->Fourier.FourierTransform(img_temp, 1);

	// 滤波
	float rh = 3, rl = 0.5, c = 5, d0 = 10; //参数
	Mat Homomorphic = calc_Homomorphic(this->Fourier.mResult.size(), rl, rh, c, d0);
	this->Fourier.mResult = this->Fourier.mResult.mul(Homomorphic);

	// 傅里叶逆变换
	Mat result;
	this->Fourier.FourierInverseTransform(result);

	//imshow(", ", result);
	//waitKey(0);

	//exp(result, result);  // TODO: Something wrong here!
	normalize(result, result, 0, 1, NORM_MINMAX);
	result.convertTo(result, CV_8U, 255);

	return ConvertToImg(img, result);

}

void Processing::DisplayEffect(const Img& img_origin, const Img& img_processed) {
	// TODO: 自动调整位置
	img_origin.DisplayImg(0, 180);
	img_processed.DisplayImg(img_origin.bmp_info.bi.biWidth + 10, 180);
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

		y1 = round(multiple * y);

		if (y == img.bmp_info.bi.biHeight - 1 || y1 > height - 1)
			y1 = height - 1;

		for (int x = 0; x < img.bmp_info.bi.biWidth; x++) {

			x1 = round(multiple * x);

			if (x == img.bmp_info.bi.biWidth - 1 || x1 > width - 1)
				x1 = width - 1;

			img_data[y1 * width + x1] = img.bmp_info.img_data[y * img.bmp_info.bi.biWidth + x];
		}
	}

	if (multiple > 1) {
		float x0, y0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (img_data[y * width + x][0] == img_data[y * width + x][1] == img_data[y * width + x][2] == 0) {

					y0 = y / multiple;
					x0 = x / multiple;

					if (y == height - 1 || y0 > img.bmp_info.bi.biHeight - 1)
						y0 = img.bmp_info.bi.biHeight - 1;

					if (x == width - 1 || x0 > img.bmp_info.bi.biWidth - 1)
						x0 = img.bmp_info.bi.biWidth - 1;

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