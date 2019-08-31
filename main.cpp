﻿#include <iostream>      
#include <vector>
#include <iterator>
#include <random>
#include <chrono>
#include "opencv2/opencv.hpp"
#include "algorithm/Fourier.h"
#include "image/Image.h"
#include "./assist/Rand.h"
#include <share.h>
#include "./assist/Timer.h"


using namespace std;
using namespace cv;

template<typename T>
void Print(const vector<vector<T>> &vt)
{
	for (const auto &e : vt)
	{
		for (const auto&i : e)
		{
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}
}
std::ostream& operator<<(std::ostream &os, const std::complex<double> &cd)
{
	os << "[" << cd.real() << "," << cd.imag() << "]";
	return os;
}

int OpencvFourier(Mat img);


// 傅里叶滤波示例
void FourierFilterExampel(const Mat &src);

int main()
{
	Mat src = imread("6.tif", 0);
	FourierFilterExampel(src);
	
	waitKey();
	system("pause");
	return 0;
}

void FourierFilterExampel(const Mat &src)
{
	assert(src.type() == CV_8UC1);

	// 初始化变量和数据
	Timer timer;
	Rand<double> _rand(1, 10);
	IPL::FourierFilter fourier_filter;

	
	// ------------------------------------- FFT ---------------------------------------
	cout << "快速傅里变换" << endl;
	Mat src_fft;
	src.copyTo(src_fft);
	int old_rows = src_fft.rows;
	int old_cols = src_fft.cols;
	// 寻找最佳图像尺寸
	int optimal_rows = IPL::Nearst2Power(src_fft.rows);
	int optimal_cols = IPL::Nearst2Power(src_fft.cols);
	resize(src_fft, src_fft, { optimal_cols,optimal_rows });

	// 生成滤波核
	auto kernel_fft = IPL::GHPF(src_fft.rows, src_fft.cols, 160);

	timer.Start();
	Mat fft = fourier_filter(src_fft, kernel_fft);
	cout << "fft: " << timer.Elapse()<<" milliseconds" << endl;

	resize(fft, fft, { old_cols,old_rows });
	imshow("fft", fft);

	cout << endl;



	// ------------------------------------- DFT ---------------------------------------
	cout << "离散傅里叶变换" << endl;
	Mat src_dft;
	src.copyTo(src_dft);
	// 生成滤波核
	auto kernel_dft = IPL::GHPF(src_dft.rows, src_dft.cols, 160);
	timer.ReSet();
	Mat dft = fourier_filter(src_dft, kernel_dft, 1);
	cout << "dft: " << timer.Elapse() << " milliseconds" << endl;
	imshow("dft", dft);
	
}



int OpencvFourier(Mat img)
{
	Mat src = img;
	if (src.empty())
	{
		return -1;
	}

	Mat src_gray;
	cvtColor(src, src_gray, CV_RGB2GRAY);//灰度图像做傅里叶变换

	int m = getOptimalDFTSize(src_gray.rows); //2,3,5的倍数有更高效率的傅里叶转换
	int n = getOptimalDFTSize(src_gray.cols);

	Mat dst;
	///把灰度图像放在左上角，在右边和下边扩展图像，扩展部分填充为0；
	copyMakeBorder(src_gray, dst, 0, m - src_gray.rows, 0, n - src_gray.cols, BORDER_CONSTANT, Scalar::all(0));
	cout << dst.size() << endl;

	//新建一个两页的array，其中第一页用扩展后的图像初始化，第二页初始化为0
	Mat planes[] = { Mat_<float>(dst), Mat::zeros(dst.size(), CV_32F) };
	Mat  completeI;
	merge(planes, 2, completeI);//把两页合成一个2通道的mat

	//对上边合成的mat进行傅里叶变换，支持原地操作，傅里叶变换结果为复数。通道1存的是实部，通道2存的是虚部。
	dft(completeI, completeI);

	split(completeI, planes);//把变换后的结果分割到各个数组的两页中，方便后续操作
	magnitude(planes[0], planes[1], planes[0]);//求傅里叶变换各频率的幅值，幅值放在第一页中。

	Mat magI = planes[0];
	//傅立叶变换的幅度值范围大到不适合在屏幕上显示。高值在屏幕上显示为白点，
	//而低值为黑点，高低值的变化无法有效分辨。为了在屏幕上凸显出高低变化的连续性，我们可以用对数尺度来替换线性尺度:
	magI += 1;
	log(magI, magI);//取对数
	magI = magI(Rect(0, 0, src_gray.cols, src_gray.rows));//前边对原始图像进行了扩展，这里把对原始图像傅里叶变换取出，剔除扩展部分。


	//这一步的目的仍然是为了显示。 现在我们有了重分布后的幅度图，
	//但是幅度值仍然超过可显示范围[0,1] 。我们使用 normalize() 函数将幅度归一化到可显示范围。
	normalize(magI, magI, 0, 1, CV_MINMAX);//傅里叶图像进行归一化。


	//重新分配象限，使（0,0）移动到图像中心，
	//在《数字图像处理》中，傅里叶变换之前要对源图像乘以（-1）^(x+y)进行中心化。
	//这是是对傅里叶变换结果进行中心化
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat tmp;
	Mat q0(magI, Rect(0, 0, cx, cy));
	Mat q1(magI, Rect(cx, 0, cx, cy));
	Mat q2(magI, Rect(0, cy, cx, cy));
	Mat q3(magI, Rect(cx, cy, cx, cy));


	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);



	namedWindow("InputImage");
	imshow("InputImage", src);

	namedWindow("SpectrumImage");
	imshow("SpectrumImage", magI);
}
