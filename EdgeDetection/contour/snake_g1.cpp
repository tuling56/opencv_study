/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	snake1.cpp
* Brief: 用能量最小化算法更新轮廓，官方示例程序
* Source:http://wiki.opencv.org.cn/index.php/Snake%E8%BD%AE%E5%BB%93%E4%BE%8B%E5%AD%90
* Status: OpenCV新版本中找不到cvSnakeImage（）这个函数，未完成
		: 解决方式：加入这个头文件 #include <opencv2/legacy/legacy.hpp>
* Date:	[3/28/2015 jmy]
************************************************************************/

#include <iostream>
#include <string.h>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <fstream>
#include <opencv2/legacy/legacy.hpp>


IplImage *image = 0; //原始图像
IplImage *image2 = 0; //原始图像copy

using namespace std;
int Thresholdness = 141;
int ialpha = 20;
int ibeta = 20;
int igamma = 20;

void onChange(int pos)
{
	if (image2) cvReleaseImage(&image2);
	if (image) cvReleaseImage(&image);

	image2 = cvLoadImage("samples//snake.jpg", 1); 
	image = cvLoadImage("samples//snake.jpg", 0);
	cvThreshold(image, image, Thresholdness, 255, CV_THRESH_BINARY); //分割域值	

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	//寻找初始化轮廓
	cvFindContours(image, storage, &contours, sizeof(CvContour),CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if (!contours) return;
	int length = contours->total;
	if (length < 1)  //为什么轮廓个数小于10就返回？
		return;
	
	CvPoint* point = new CvPoint[length]; //分配轮廓点
	CvSeqReader reader;
	CvPoint pt = cvPoint(0, 0);;
	CvSeq *contour2 = contours;
	cvStartReadSeq(contour2, &reader);
	for (int i = 0; i < length; i++)
	{
		CV_READ_SEQ_ELEM(pt, reader);
		point[i] = pt;
	}
	cvReleaseMemStorage(&storage);

	//显示轮廓曲线
	for (int i = 0; i < length; i++)
	{
		int j = (i + 1) % length;
		cvLine(image2, point[i], point[j], CV_RGB(0, 0, 255), 1, 8, 0);
	}

	float alpha = ialpha / 100.0f;
	float beta = ibeta / 100.0f;
	float gamma = igamma / 100.0f;

	CvSize size;
	size.width = 3;
	size.height = 3;
	CvTermCriteria criteria;
	criteria.type = CV_TERMCRIT_ITER;
	criteria.max_iter = 1000;
	criteria.epsilon = 0.1;
	cvSnakeImage(image, point, length, &alpha, &beta, &gamma, CV_VALUE, size, criteria, 0);

	//显示曲线
	for (int i = 0; i < length; i++)
	{
		int j = (i + 1) % length;
		cvLine(image2, point[i], point[j], CV_RGB(0, 255, 0), 1, 8, 0);
	}
	delete[]point;

}


////功能测试区
int snake2(int argc, char* argv[])
{
	cvNamedWindow("Snake", 0);
	cvCreateTrackbar("Thd", "Snake", &Thresholdness, 255, onChange);
	cvCreateTrackbar("alpha", "Snake", &ialpha, 100, onChange);
	cvCreateTrackbar("beta", "Snake", &ibeta, 100, onChange);
	cvCreateTrackbar("gamma", "Snake", &igamma, 100, onChange);
	cvResizeWindow("Snake", 700, 400);
	
	onChange(0);

	for (;;)
	{
		if (cvWaitKey(40) == 27) break;
		cvShowImage("Snake", image2);
	}

	return 0;
}