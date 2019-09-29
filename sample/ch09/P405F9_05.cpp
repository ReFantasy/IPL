#include <iostream>      
#include "opencv2/opencv.hpp"
#include "utility/utility.hpp"
#include "algorithm/algorithm.hpp"



using namespace std;
using namespace cv;

int main(int argc, char*argv[])
{
	Mat src = imread("../data/DIP3E_Original_Images_CH09/Fig0905(a)(wirebond-mask).tif", 0);
	imshow("a", src);


	Mat b;
	cv::erode(src, b, getStructuringElement(cv::MORPH_RECT, { 11,11 }));
	imshow("b", b);

	Mat c;
	cv::erode(src, c, getStructuringElement(cv::MORPH_RECT, { 15,15 }));
	imshow("c", c);

	Mat d;
	cv::erode(src, d, getStructuringElement(cv::MORPH_RECT, { 45,45 }));
	imshow("d", d);
	

	Mat mask = getStructuringElement(cv::MORPH_RECT, { 3,3 });
	cout << mask << endl;
	waitKey();
	return 0;
}

