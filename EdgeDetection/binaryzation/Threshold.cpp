/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	binaryzation.cpp
* Brief: 滑动条调节阈值，进行图像二值化
* Source:http://blog.csdn.net/morewindows/article/details/8239678
* Status: 
* Date:	[3/12/2015 jmy]
************************************************************************/

#include <opencv2/opencv.hpp>

using namespace std;

IplImage *g_pGrayImage = NULL;
IplImage *g_pBinaryImage = NULL;
const char *pstrWindowsBinaryTitle = "二值图";

void binaryzation_trackbar(int pos)
{
	// 转为二值图
	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);
	// 显示二值图
	cvShowImage(pstrWindowsBinaryTitle, g_pBinaryImage);
}

int binaryzation(char*path)
{	
	const char *pstrWindowsSrcTitle = "原图";
	const char *pstrWindowsToolBarName = "二值图阈值";

	// 从文件中加载原图
	IplImage *pSrcImage = cvLoadImage(path, CV_LOAD_IMAGE_UNCHANGED);

	// 如果不是灰度图则转为灰度图
	if (pSrcImage->nChannels!=1)
	{
		g_pGrayImage =  cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
		cvCvtColor(pSrcImage, g_pGrayImage, CV_BGR2GRAY);
	}


	// 创建二值图
	g_pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);

	// 显示原图
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsSrcTitle, pSrcImage);
	// 创建二值图窗口
	cvNamedWindow(pstrWindowsBinaryTitle, CV_WINDOW_AUTOSIZE);

	// 滑动条	
	int nThreshold = 0;
	cvCreateTrackbar(pstrWindowsToolBarName, pstrWindowsBinaryTitle, &nThreshold, 254, binaryzation_trackbar);

	binaryzation_trackbar(1);

	cvWaitKey(0);

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(pstrWindowsBinaryTitle);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&g_pGrayImage);
	cvReleaseImage(&g_pBinaryImage);
	
	return 0;
}