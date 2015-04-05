/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	snake.cpp
* Brief: 最大熵阈值分割
* Source:http://blog.csdn.net/crond123/article/details/3958807
* Status: 未完成
* Date:	[3/28/2015 jmy]
************************************************************************/

#include "cv.h"
#include "highgui.h"
#include <opencv2/legacy/legacy.hpp>

#include <cmath>
#include <iostream>
#include <vector>
#include <cassert>

// #pragma comment(lib,"highgui.lib")
// #pragma comment(lib,"cv.lib") 
// #pragma comment(lib,"cvaux.lib") 
// #pragma comment(lib,"cxcore.lib") 

std::vector<CvPoint>   InitContour;
IplImage* temp;  //= cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

void showContent(IplImage * img)
{
	if (temp == NULL)
		temp = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cvCopyImage(img, temp);
	for (int i = 0; i < InitContour.size(); i++)
	{
		cvCircle(temp, InitContour[i], 2, cvScalarAll(155));
		int j = (i + 1) % InitContour.size();
		cvLine(temp, InitContour[i], InitContour[j], cvScalarAll(100), 1);
	}
	cvSaveImage("sef.jpg", temp);
	cvShowImage("srcImage", temp);
}

double wcvDistance(CvPoint &pt1, CvPoint &pt2)
{
	double distance = sqrt(pow((double)(pt1.x - pt2.x), 2) + pow((double)(pt1.y - pt2.y), 2));
	return distance;
}

void CaculateDiff(std::vector<CvPoint> &src, std::vector<double> &diff)
{
	int size = src.size();
	diff.clear();
	diff.reserve(size);
	diff.resize(size, 0.0);
	for (int i = 0; i < size; i++)
	{
		int j = (i + 1) % size;
		diff[i] = wcvDistance(src[i], src[j]);
	}
}

void removeNearPoints(std::vector<CvPoint> &src, std::vector<CvPoint> &dst, double max_length)
{
	std::vector<CvPoint> result;
	result.clear();
	std::vector<CvPoint> temp;
	temp.clear();
	temp = src;
	std::vector<double> diff;
	int removed = 1;
	while (removed != 0)
	{
		//diff.clear();
		result.clear();
		removed = 0;
		CaculateDiff(temp, diff);
		int srcsize = temp.size();
		for (int i = 0; i < srcsize; i++)
		{
			if (diff[i] < max_length && (i % 2 == 0))
			{
				removed++;
				continue;
			}
			result.push_back(temp[i]);
		}
		temp.clear();
		temp = result;
	}
	dst.clear();
	dst = result;
	return;
}

void contourInterpolation(std::vector<CvPoint> &src, std::vector<CvPoint> &dst)
{
	std::vector<CvPoint> temp;
	std::vector<double> diff;
	CaculateDiff(src, diff);
}


void on_mouse(int event, int x, int y, int flags, void* ptr)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		InitContour.push_back(cvPoint(x, y));
		showContent((IplImage *)ptr);
	}
}

//功能测试区
int snake1(int argc, char* argv[])
{
	IplImage * srcimage = NULL;
	if (argc == 2 &&(srcimage = cvLoadImage((char *)argv[1], CV_LOAD_IMAGE_GRAYSCALE)) != 0);
	else
	{
		srcimage = cvLoadImage("samples//snake.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	}
	if (srcimage == NULL)
	{
		std::cout << "error file name" << std::endl;
		return -1;
	}
	InitContour.clear();
	cvNamedWindow("srcImage");
	cvShowImage("srcImage", srcimage);
	cvSetMouseCallback("srcImage", on_mouse, srcimage);
	char c;
	while (char c = cvWaitKey(0))
		if (c == 's' || c == 'S')		
			break;

	float alpha = 0.8;
	float beta = 0.5;
	float gamma = 1.0;
	
	CvSize size;
	size.width = 3;
	size.height = 3;
	CvTermCriteria criteria;
	criteria.type = CV_TERMCRIT_ITER;
	criteria.max_iter = 500;
	criteria.epsilon = 0.1;
	int itetime = 100;
	for (int ite = 0; ite < itetime; ite++)
	{
		CvPoint*  pts = new  CvPoint[InitContour.size()];
		for (int i = 0; i < InitContour.size(); i++)
		{
			pts[i] = InitContour[i];
		}
		cvSnakeImage(srcimage, pts, InitContour.size(),&alpha,&beta,&gamma, CV_VALUE, size, criteria, 1);
		int size = InitContour.size();
		InitContour.clear();
		for (int i = 0; i < size; i++)
		{
			InitContour.push_back(pts[i]);
			int next = (i + 1) % size;
			CvPoint ne = pts[next];
			if (size < 500)
			{
				CvPoint mid = cvPoint((pts[i].x + ne.x) / 2, (pts[i].y + ne.y) / 2);
				InitContour.push_back(mid);
			}
		}
		removeNearPoints(InitContour, InitContour, 10);
		delete[]pts;
		//s			Sleep(100);
		//		showContent(srcimage);

	}
	removeNearPoints(InitContour, InitContour, 10);
	showContent(srcimage);
	cvWaitKey();

	cvReleaseImage(&srcimage);
	cvReleaseImage(&temp);
	return 0;
}