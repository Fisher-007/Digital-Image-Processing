#include "processing.h"
#include "message.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utils/logger.hpp>
using namespace cv;

#define PI 3.1415926


// TODO: ��������������⣨��ͬͼƬ�������Ͳ�ͬ��


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
		ExitMessage("ͼ���ļ���ȡʧ�ܣ�");
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
	// TODO: ���������Ƶ��ͼ

	Mat result;

	idft(this->mResult, result, DFT_REAL_OUTPUT);
	normalize(result, result, 0, 1, NORM_MINMAX); // TODO: �޷�100%��ԭ��&& 32F��ɫ��ʾ���⣿

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

	//	Mat q00(mRe, Rect(0, 0, cx, cy));   // ��������
	//	Mat q01(mRe, Rect(cx, 0, cx, cy));  // ��������
	//	Mat q02(mRe, Rect(0, cy, cx, cy));  // ��������
	//	Mat q03(mRe, Rect(cx, cy, cx, cy)); // ��������
	//	q00.copyTo(tmp); q03.copyTo(q00); tmp.copyTo(q03);//���������½��н���
	//	q01.copyTo(tmp); q02.copyTo(q01); tmp.copyTo(q02);//���������½��н���

	//	Mat q10(mIm, Rect(0, 0, cx, cy));   // ��������
	//	Mat q11(mIm, Rect(cx, 0, cx, cy));  // ��������
	//	Mat q12(mIm, Rect(0, cy, cx, cy));  // ��������
	//	Mat q13(mIm, Rect(cx, cy, cx, cy)); // ��������
	//	q10.copyTo(tmp); q13.copyTo(q10); tmp.copyTo(q13);//���������½��н���
	//	q11.copyTo(tmp); q12.copyTo(q11); tmp.copyTo(q12);//���������½��н���

	//	imshow("R1", mRe);
	//	imshow("I1", mIm);

	//	mForFourier[0] = mRe;
	//	mForFourier[1] = mIm;
	//	merge(mForFourier, 2, this->mResult);//������Ҷ�任������Ļ�
	//}
}

void Processing::Fourier::FourierInverseTransform(Mat& result) {
	// TODO: ���������Ƶ��ͼ

	idft(this->mResult, result, DFT_REAL_OUTPUT);
	result = result(Rect(0, 0, result.cols - dn, result.rows - dm));
}

Img Processing::HistogramEqualization(const Img& img) {

	setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	// Ŀǰֻ֧��BMP�ļ�
	if (img.img_type != "bmp")
		ExitMessage("Ŀǰ��֧�ָ�ͼ���ʽ��");

	Mat img_origin = imread(img.file_path);
	if (img_origin.empty()) {
		ExitMessage("ͼ���ȡʧ�� ");
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
			//̬ͬ�˲����ݺ���
			p[j] = (gamma_H - gamma_L) * (1 - std::exp(-c * d_2 / (D0 * D0))) + gamma_L;
		}
	}

	//����˫ͨ��ͼ�񣬱����븴��ͼ�����
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
		ExitMessage("ͼ���ȡʧ�� ");
	}

	Mat img_temp;
	img_origin.convertTo(img_temp, CV_32F, 1 / 255.0);

	// cvtColor(image, image_gray, COLOR_BGR2GRAY); //ת��Ϊ�Ҷ�ͼ

	// ����ln�任
	log(img_temp + 1, img_temp); // Ϊʲô����Ҫ�� 1

	// ����Ҷ�任
	this->Fourier.FourierTransform(img_temp, 1);

	// �˲�
	float rh = 3, rl = 0.5, c = 5, d0 = 10; //����
	Mat Homomorphic = calc_Homomorphic(this->Fourier.mResult.size(), rl, rh, c, d0);
	this->Fourier.mResult = this->Fourier.mResult.mul(Homomorphic);

	// ����Ҷ��任
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
	// TODO: �Զ�����λ��
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
	img.NewImgInfo(output, img_data);

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
	img.NewImgInfo(output, img_data);

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
	img.NewImgInfo(output, img_data, width, height);

	return output;
}


// TODO: ���ϲ�ͬͼ������
Img Processing::GrayLevelWindow(const Img& img, int pos, int width) {
	vector<unsigned short> img_data;
	unsigned short temp;
	for (int i = 0; i < img.custom_info.width * img.custom_info.height; i++) {
		temp = img.custom_info.img_data.at(i);
		if (temp < pos - width / 2)
			img_data.push_back(0);
		else if (temp > pos + width / 2)
			img_data.push_back(255);
		else
			img_data.push_back((temp - pos + width / 2) * 256 / width);
	}

	Img output;
	img.NewImgInfo(output, img_data);

	return output;
}

Img Processing::EnhanceDetails(const Img& img) {
	vector<unsigned short> img_data;
	unsigned short temp;

	Img output;
	img.NewImgInfo(output, img_data);

	return output;
}


void saltAndPepper(Mat& image, int n) {
	for (int k = 0; k < n; ++k) {
		//���ȷ��ͼ����λ��
		int i, j;
		i = rand() % image.cols;//ȡ�������㣬��֤��ͼ���������
		j = rand() % image.rows;//ȡ�������㣬��֤��ͼ���������
		int write_black = rand() % 2;//�ж�Ϊ��ɫ�������Ǻ�ɫ�����ı���
		if (write_black == 0) {//��Ӱ�ɫ����
			if (image.type() == CV_8UC1) {//����Ҷ�ͼ��
				image.at<uchar>(j, i) = 255;//��ɫ����
			}
			else if (image.type() == CV_8UC3) {//�����ɫͼ��
				image.at<Vec3b>(j, i)[0] = 255;//Vec3bΪopencv�����3��ֵ����������
				image.at<Vec3b>(j, i)[1] = 255;//[]�ƶ�ͨ����B:0,G:1,R:2
				image.at<Vec3b>(j, i)[2] = 255;
			}
		}
		else {//��Ӻ�����
			if (image.type() == CV_8UC1) {
				image.at<uchar>(j, i) = 0;
			}
			else if (image.type() == CV_8UC3) {
				image.at<Vec3b>(j, i)[0] = 0;//Vec3bΪopencv�����3��ֵ����������
				image.at<Vec3b>(j, i)[1] = 0;//[]�ƶ�ͨ����B:0,G:1,R:2
				image.at<Vec3b>(j, i)[2] = 0;
			}
		}
	}
}


//ʵ���˲�����
void AdaptiveLocalNoiseReductionFilter(Mat img_input, Mat& img_output, int m, int n)
{
	img_output = img_input.clone();
	Mat sortarray(1, m * n, CV_8U);  //�ֲ����ػҶȾ���

	//1��Ϊ�˱�֤ͼ��ı�ԵҲ�ܹ����˲�������������չͼ���Ե����չ����Ϊ����
	copyMakeBorder(img_input, img_input, (m - 1) / 2, (m - 1) / 2, (n - 1) / 2, (n - 1) / 2, BORDER_REFLECT);

	//2������ͼ�񷽲�
	Mat mat_mean1, mat_stddev1, mat_mean2, mat_stddev2; //ͼ���ֵ��׼����󣬾ֲ���ֵ��׼�����
	meanStdDev(img_input, mat_mean1, mat_stddev1); //meanStdDev��ȡ�����ƽ��ֵ�ͱ�׼��
	double stddev1, mean2, stddev2;  //ͼ���׼��ֲ���ֵ�ͱ�׼��
	stddev1 = mat_stddev1.at<double>(0, 0);//ͼ���׼��


	//3������Ӧ�ֲ������˲�
	for (int i = (m - 1) / 2; i < img_input.rows - (m - 1) / 2; i++)
	{
		for (int j = (n - 1) / 2; j < img_input.cols - (n - 1) / 2; j++)
		{
			int h = 0;
			for (int x = -(m - 1) / 2; x <= (m - 1) / 2; x++)
			{
				for (int y = -(n - 1) / 2; y <= (n - 1) / 2; y++)
				{
					sortarray.at<uchar>(h) = img_input.at<uchar>(i + x, j + y);
					h++;
				}
			}

			//����ֲ���ֵ�ͷ���
			meanStdDev(sortarray, mat_mean2, mat_stddev2);
			stddev2 = mat_stddev2.at<double>(0, 0);  //�ֲ���׼��
			mean2 = mat_mean2.at<double>(0, 0);  //�ֲ���ֵ

			//�˲���
			double k = (stddev1 * stddev1) / (stddev2 * stddev2 + 0.00001);
			if (k <= 1)
			{
				img_output.at<uchar>(i - (m - 1) / 2, j - (n - 1) / 2) = img_input.at<uchar>(i, j) - k * (img_input.at<uchar>(i, j) - mean2);
			}
			else
			{
				img_output.at<uchar>(i - (m - 1) / 2, j - (n - 1) / 2) = mean2;
			}
		}
	}
}


Mat mediaFilter(const Mat& src, int ksize = 3) {
	Mat dst;
	medianBlur(src, dst, ksize);
	return dst;
}


int main2()
{
	Mat image, image_gray, image_output;   //��������ͼ�񣬻Ҷ�ͼ�����ͼ��
	image = imread("data/test.bmp");  //��ȡͼ��
	if (image.empty())
	{
		cout << "��ȡ����" << '\n';
		return -1;
	}
	imshow("image", image);

	saltAndPepper(image, 10000);

	//ת��Ϊ�Ҷ�ͼ��
	cvtColor(image, image_gray, COLOR_BGR2GRAY);

	mediaFilter(image_gray);
	imshow("media filter", image_gray);

	//�Լ���д���˲�����
	AdaptiveLocalNoiseReductionFilter(image_gray, image_output, 3, 3);
	imshow("adaptive media filter", image_output);

	waitKey(0);  //��ͣ������ͼ����ʾ���ȴ���������
	return 0;
}