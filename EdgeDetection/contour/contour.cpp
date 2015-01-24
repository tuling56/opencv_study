/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	contour.cpp
* Brief: 该文件包含两个函数，都是自己绘图，然后检测轮廓
* Source:http://blog.csdn.net/MoreWindows
* Status: 
* Date:	[1/14/2015 jmy]
************************************************************************/


#include <opencv2/opencv.hpp>
using namespace std;

/*
 *	功能：自己画图，然后进行轮廓检测，需要注意的几个问题
 *      （1）：OpenCV的findContour（）函数寻找的是白色区域为轮廓
 *  输出：
 *  状态：
 */

int contour1()
{	
	const char *pstrWindowsSrcTitle = "原图(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsOutLineTitle = "轮廓图(http://blog.csdn.net/MoreWindows)";

	const int IMAGE_WIDTH = 400;
	const int IMAGE_HEIGHT = 200;

	// 创建图像
	IplImage *pSrcImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	//cvRectangle(pSrcImage, cvPoint(0, 0), cvPoint(pSrcImage->width, pSrcImage->height),
		       // CV_RGB(255, 255, 255), CV_FILLED);// 填充成白色
	cvSetZero(pSrcImage);
	
	// 画圆
	CvPoint ptCircleCenter = cvPoint(IMAGE_WIDTH / 4, IMAGE_HEIGHT / 2);
	int nRadius = 80;
	cvCircle(pSrcImage, ptCircleCenter, nRadius, CV_RGB(255, 255, 255), CV_FILLED);
	ptCircleCenter = cvPoint(IMAGE_WIDTH / 4, IMAGE_HEIGHT / 2);
	nRadius = 30;
	cvCircle(pSrcImage, ptCircleCenter, nRadius, CV_RGB(0, 0, 0), CV_FILLED);
	
	// 画矩形
	CvPoint ptLeftTop = cvPoint(IMAGE_WIDTH / 2 + 20, 20);
	CvPoint ptRightBottom = cvPoint(IMAGE_WIDTH - 20, IMAGE_HEIGHT - 20);
	cvRectangle(pSrcImage, ptLeftTop, ptRightBottom, CV_RGB(255, 255, 255), CV_FILLED);
	ptLeftTop = cvPoint(IMAGE_WIDTH / 2 + 60, 40);
	ptRightBottom = cvPoint(IMAGE_WIDTH - 60, IMAGE_HEIGHT - 40);
	cvRectangle(pSrcImage, ptLeftTop, ptRightBottom, CV_RGB(0, 0, 0), CV_FILLED);
	
	// 显示原图
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsSrcTitle, pSrcImage);


	// 转为灰度图
	IplImage *pGrayImage =  cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
	// 转为二值图
	IplImage *pBinaryImage = cvCreateImage(cvGetSize(pGrayImage), IPL_DEPTH_8U, 1);
	cvThreshold(pGrayImage, pBinaryImage, 250, 255, CV_THRESH_BINARY);


	// 检索轮廓并返回检测到的轮廓的个数
	CvMemStorage *pcvMStorage = cvCreateMemStorage();
	CvSeq *pcvSeq = NULL;
	/*要修改的也正是这个地方*/
	cvFindContours(pBinaryImage, pcvMStorage, &pcvSeq, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// 画轮廓图
	IplImage *pOutlineImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 3);
	cvRectangle(pOutlineImage, cvPoint(0, 0), cvPoint(pOutlineImage->width, pOutlineImage->height), 
		        CV_RGB(255, 255, 255), CV_FILLED);// 填充成白色
	int nLevels = 3;
	/*要修改的也正是这个地方,外层红，内层绿*/
	cvDrawContours(pOutlineImage, pcvSeq, CV_RGB(255,0,0), CV_RGB(0,255,0), nLevels, 2);
	
	
	// 显示轮廓图
	cvNamedWindow(pstrWindowsOutLineTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsOutLineTitle, pOutlineImage);
	cvWaitKey(0);
	cvReleaseMemStorage(&pcvMStorage);
	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(pstrWindowsOutLineTitle);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&pGrayImage);
	cvReleaseImage(&pBinaryImage);
	cvReleaseImage(&pOutlineImage);
	return 0;
}




/*
 *	功能：轮廓检测，添加了滑动条
 *  输入：
 *  输出：
 *  状态：
 */
IplImage *greyimg = NULL;
const char *binWndName = "二值图(http://blog.csdn.net/MoreWindows)";
const char *contourWndName = "轮廓图(http://blog.csdn.net/MoreWindows)";
CvSeq *g_pcvSeq = NULL;

void contour2_trackbar(int n_level)
{
	// 转为二值图
	IplImage *pBinaryImage = cvCreateImage(cvGetSize(greyimg), IPL_DEPTH_8U, 1);
	cvThreshold(greyimg, pBinaryImage, 50, 255, CV_THRESH_BINARY);
	// 显示二值图
	cvShowImage(binWndName, pBinaryImage);

	CvMemStorage* cvMStorage = cvCreateMemStorage();
	// 检索轮廓并返回检测到的轮廓的个数
	cvFindContours(pBinaryImage,cvMStorage, &g_pcvSeq,sizeof(CvContour),CV_RETR_EXTERNAL);

	IplImage *pOutlineImage = cvCreateImage(cvGetSize(greyimg), IPL_DEPTH_8U, 3);
	//int _levels = 3;
	cvZero(pOutlineImage);
	cvDrawContours(pOutlineImage, g_pcvSeq, CV_RGB(255,0,0), CV_RGB(0,255,0), n_level-3);
	cvShowImage(contourWndName, pOutlineImage);

	cvReleaseMemStorage(&cvMStorage);
	cvReleaseImage(&pBinaryImage);
	cvReleaseImage(&pOutlineImage);
}

int contour2(char*path)
{	
	const char *pstrWindowsSrcTitle = "原图(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsToolBarName = "level";

	IplImage *pSrcImage = cvLoadImage(path, CV_LOAD_IMAGE_UNCHANGED);
	if (!pSrcImage){
		printf("加载失败\n");
		exit(0);
	}
	// 显示原图
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsSrcTitle, pSrcImage);

	// 转为灰度图
	greyimg =  cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, greyimg, CV_BGR2GRAY);

	// 创建二值图和轮廓图窗口
	cvNamedWindow(binWndName, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(contourWndName, CV_WINDOW_AUTOSIZE);

	// 滑动条	
	int nThreshold = 0;
	cvCreateTrackbar(pstrWindowsToolBarName, binWndName, &nThreshold, 7, contour2_trackbar);
	contour2_trackbar(1);

	cvWaitKey(0);

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(binWndName);
	cvDestroyWindow(contourWndName);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&greyimg);
	return 0;
}

//功能测试区
//int main()
//{
//	//contour1();
//	contour2("samples/contour/contour.jpg");
//	return 0;
//}