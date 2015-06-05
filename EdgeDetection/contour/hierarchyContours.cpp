/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	hierarchyContours.cpp
* Brief: ²ã´ÎÂÖÀª
* Reference:
* Status: 
* Date:	[5/13/2015 jmy]
************************************************************************/
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

int hierarchyContours(Mat src)
{
	assert(src.depth()==CV_8U);

	Mat demo;// (src.rows, src.cols, CV_8UC3, Scalar(0));
	cvtColor(src, demo, CV_GRAY2BGR);


	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	
	int idx = 0;
	for (; idx > 0;idx=hierarchy[idx][0])
	{
		drawContours(demo, contours, idx, Scalar(255, 0, 0), 1, 8, hierarchy, 1);
	}

	imshow("HierarchyContours", demo);
	waitKey();

	return 0;
}

int main()
{
	Mat src = imread("samples\\contour\\contour.jpg",0);
	hierarchyContours(src);

	return 0;

}