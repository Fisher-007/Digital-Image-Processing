#include "processing.h"
#include "message.h"
#include <opencv2/opencv.hpp>
using namespace cv;


Img Processing::FourierTransform(const Img& img)
{
	Mat img_origin = imread(img.get_file_path(), 0);
	if (img_origin.data == 0) {
		ExitMessage("图像文件读取失败！");
	}
	// namedWindow("The original image", WINDOW_NORMAL);
	// imshow("The original image", img_origin);

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

	// namedWindow("The Fourier transform", WINDOW_NORMAL);
	// imshow("The Fourier transform", mResult);
	// waitKey();
	// destroyAllWindows();

	vector<vector<uchar>> img_result;
	vector<uchar> sub_img;
	for (int i = 0; i < mResult.rows; ++i)
		img_result[i] = sub_img;    // 容器直接赋值是深拷贝

	for (int i = 0; i < img_origin.rows; ++i)
		for (int j = 0; j < img_origin.cols; ++j)
			img_result[i][j] = img_origin.at<uchar>(i, j);

	return img;
}

Img Processing::FourierInverseTransform(const Img& img) {
	return img;
}

Img Processing::HistogramEqualization(const Img& img) {
	return img;
}