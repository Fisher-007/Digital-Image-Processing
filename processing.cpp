#include "processing.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


Img Processing::FourierTransform(const Img& img)
{
	Mat mImage = imread(img.get_file_path(), 0);
	if (mImage.data == 0)
	{
		cerr << "Image reading error" << endl;
		system("pause");
		exit(0);
	}
	namedWindow("The original image", WINDOW_NORMAL);
	imshow("The original image", mImage);

	//Extending image
	int m = getOptimalDFTSize(mImage.rows);
	int n = getOptimalDFTSize(mImage.cols);
	copyMakeBorder(mImage, mImage, 0, m - mImage.rows, 0, n - mImage.cols, BORDER_CONSTANT, Scalar(0));

	//Fourier transform
	Mat mFourier(mImage.rows + m, mImage.cols + n, CV_32FC2, Scalar(0, 0));
	Mat mForFourier[] = { Mat_<float>(mImage), Mat::zeros(mImage.size(), CV_32F) };
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

	Mat mResult(mImage.rows, mImage.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < mImage.rows; i++)
	{
		uchar* pResult = mResult.ptr<uchar>(i);
		float* pAmplitude = mAmplitude.ptr<float>(i);
		for (int j = 0; j < mImage.cols; j++)
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
	return img;
}