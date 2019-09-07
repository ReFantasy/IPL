﻿/************************************************************************
 *
 *  自定义图像类型与OpenCV图像类型转换
 *
 ***********************************************************************/
#ifndef __FILTER_H__
#define __FILTER_H__
#include <vector>
#include <string>
#include "../algorithm/algorithm.hpp"
#include "image_convert.h"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;


namespace IPL
{
	// 傅里叶滤波
	class FourierFilter
	{
		using Complex2D = vector<vector<std::complex<double>>>;
		
	public:
		FourierFilter() = default;
		Mat operator()(const Mat &src, const vector<vector<double>> &gauss, int ft_type = 0);

	private:
		vector<vector<double>> ImageData(const Mat &img);
		vector<vector<double>> ConvertToImage(const Complex2D&);
		Mat Out8UC1(const vector<vector<double>> &d);

	private:
		Fourier fourier;
		vector<vector<std::complex<double>>> _src_fourier_res;
		vector<vector<std::complex<double>>> _dst_fourier_res;

		vector<vector<std::complex<double>>> filter(const vector<vector<std::complex<double>>> &fourier, const vector<vector<double>> &gauss);

	};

	// 归一化的二维浮点数组转为CV_8UC1类型图像
	cv::Mat NormalizeVec2DToMat(const vector<vector<double>> &vec2d);
	
	/** \brief Arithmetic mean filtering
	 *  \param[in] src image data
	 *  \param[in] kern_size the size of filter
	 *  \return the filtered image
	 */
	cv::Mat MeanBlurArithmetic(const cv::Mat &_src, int kern_size = 3);

	/** \brief Geometry mean filtering
	 *  \param[in] src image data
	 *  \param[in] kern_size the size of filter
	 *  \return the filtered image
	 */
	cv::Mat MeanBlurGeometry(const cv::Mat &_src, int kern_size = 3);

	/** \brief Harmonic mean filtering
	 *  \param[in] src image data
	 *  \param[in] kern_size the size of filter
	 *  \return the filtered image
	 */
	cv::Mat MeanBlurHarmonic(const Mat &_src, int kern_size = 3);

	/** \brief Inverse Harmonic mean filtering
	 *  \param[in] src image data
	 *  \param[in] kern_size the size of filter
	 *  \parem[in] Q the order of the filter
	 *  \return the filtered image
	 */
	cv::Mat MeanBlurInverseHarmonic(const Mat &_src, int kern_size, double Q = 0);
	



}
#endif//__FILTER_H__