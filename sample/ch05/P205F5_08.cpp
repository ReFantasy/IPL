﻿#include <iostream>      
#include <vector>
#include "opencv2/opencv.hpp"

#include "algorithm/algorithm.hpp"
#include "utility/utility.hpp"



using namespace std;


int main(int argc, char *argv[])
{
	Mat a = imread("../data/DIP3E_Original_Images_CH05/Fig0508(a)(circuit-board-pepper-prob-pt1).tif", 0);
	Mat b = imread("../data/DIP3E_Original_Images_CH05/Fig0508(b)(circuit-board-salt-prob-pt1).tif", 0);
	auto c = IPL::BlurInverseHarmonic(a, 3, 1.5);
	auto d = IPL::BlurInverseHarmonic(b, 3, -1.5);
	

	imshow("a", a);
	imshow("b", b);
	imshow("c", c);
	imshow("d", d);


	waitKey();
	return 0;
}




