/*
 *功能：运动检测（前景检测）
 *来自：http://blog.csdn.net/likezhaobin/article/details/6912194
 *Info:[11/12/2014 jmy]
 */

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int diff1_main()
{
	//声明IplImage指针  
	IplImage* pFrame = NULL;   
	IplImage* pFrImg = NULL;  
	IplImage* pBkImg = NULL;  
	CvMat* pFrameMat = NULL;  
	CvMat* pFrMat = NULL;  
	CvMat* pBkMat = NULL;  
	CvCapture* pCapture = NULL;  
	int nFrmNum = 0;  
	
	//创建窗口  
	cvNamedWindow("video", 1);  
	cvNamedWindow("background",1);  
	cvNamedWindow("foreground",1);  
	
	//使窗口有序排列  
	cvMoveWindow("video", 30, 0);  
	cvMoveWindow("background", 360, 0);  
	cvMoveWindow("foreground", 690, 0);  
	pCapture = cvCaptureFromAVI("bike.avi");   //读入已有视频用此句  
	//pCapture = cvCaptureFromCAM(0);           //从摄像头读入视频用此  
	if (!pCapture)
	{
		cout<<"打开失败"<<endl;
		return -1;
	}
	
	while(pFrame = cvQueryFrame( pCapture ))  
	{  
		nFrmNum++;  
		//如果是第一帧，需要申请内存，并初始化  
		if(nFrmNum == 1)  
		{  
			pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),IPL_DEPTH_8U,1);  
			pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1);  
			pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);  
			pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);  
			pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);  
			//转化成单通道图像再处理  
			cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);  
			cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);  
			cvConvert(pFrImg, pFrameMat);  
			cvConvert(pFrImg, pFrMat);  
			cvConvert(pFrImg, pBkMat);  
		}  
		else  
		{  
			cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);  
			cvConvert(pFrImg, pFrameMat);  
			//先做高斯滤波，以平滑图像  
			//cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);  
			//当前帧跟背景图相减  
			cvAbsDiff(pFrameMat, pBkMat, pFrMat);  
			//二值化前景图  
			cvThreshold(pFrMat, pFrImg, 10, 255.0, CV_THRESH_BINARY);  
			//更新背景  
			cvRunningAvg(pFrameMat, pBkMat, 0.003, 0);  
			//将背景转化为图像格式，用以显示  
			cvConvert(pBkMat, pBkImg);  
			
			//以下三语句控制图像反转的
			//pFrame->origin = IPL_ORIGIN_BL;  
			//pFrImg->origin = IPL_ORIGIN_BL;  
			//pBkImg->origin = IPL_ORIGIN_BL;  

			cvShowImage("video", pFrame);  
			cvShowImage("background", pBkImg);  
			cvShowImage("foreground", pFrImg);  
			//如果有按键事件，则跳出循环  
			//此等待也为cvShowImage函数提供时间完成显示  
			//等待时间可以根据CPU速度调整  
			if( cvWaitKey(2) >= 0 )  
				break;  
		}  
	}  

	//销毁窗口  
	cvDestroyWindow("video");  
	cvDestroyWindow("background");  
	cvDestroyWindow("foreground");  
	
	//释放图像和矩阵  
	cvReleaseImage(&pFrImg);  
	cvReleaseImage(&pBkImg);  
	cvReleaseMat(&pFrameMat);  
	cvReleaseMat(&pFrMat);  
	cvReleaseMat(&pBkMat);  
	cvReleaseCapture(&pCapture);  

	return 0;

}