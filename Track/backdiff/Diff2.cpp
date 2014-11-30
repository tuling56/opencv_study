/*
 *功能：运动检测，前景检测
 *来自：http://blog.csdn.net/augusdi/article/details/9020009
 *Info:[11/12/2014 jmy]
 */

#define  Switch 1

#if Switch

#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

//#pragma comment(lib, "cv.lib")
//#pragma comment(lib, "cxcore.lib")
//#pragma comment(lib, "highgui.lib")

int diff2_main( int argc, char** argv )
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


	if( !(pCapture = cvCaptureFromAVI("bike.avi")))
	{
		//pCapture = cvCaptureFromCAM(-1))
		fprintf(stderr, "Can not open camera.\n");
		return -2;
	}


	//逐帧读取视频
	while(pFrame = cvQueryFrame( pCapture ))
	{
		nFrmNum++;

		//如果是第一帧，需要申请内存，并初始化
		if(nFrmNum == 1)
		{
			pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);

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
			//高斯滤波先，以平滑图像
			//cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

			//当前帧跟背景图相减
			cvAbsDiff(pFrameMat, pBkMat, pFrMat);

			//二值化前景图
			cvThreshold(pFrMat, pFrImg, 60, 255.0, CV_THRESH_BINARY);

			//进行形态学滤波，去掉噪音  
			//cvErode(pFrImg, pFrImg, 0, 1);
			//cvDilate(pFrImg, pFrImg, 0, 1);

			//更新背景
			cvRunningAvg(pFrameMat, pBkMat, 0.003, 0);
			//将背景转化为图像格式，用以显示
			cvConvert(pBkMat, pBkImg);

			//显示图像
			cvShowImage("video", pFrame);
			cvShowImage("background", pBkImg);
			cvShowImage("foreground", pFrImg);

			//如果有按键事件，则跳出循环
			//此等待也为cvShowImage函数提供时间完成显示
			//等待时间可以根据CPU速度调整
			if( cvWaitKey(20) >= 0 )
			{
				break;
			}
		}
	}
	cvWaitKey();

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
#else

#include <cv.h>
#include <highgui.h>

//#pragma comment(lib, "cv.lib")
//#pragma comment(lib, "cxcore.lib")
//#pragma comment(lib, "highgui.lib")

int main( int argc, char** argv )
{
	CvCapture* capture = NULL;
	IplImage *frame = NULL;

	IplImage  *frame_last = NULL;//存储上一帧
	IplImage  *m_out_image = NULL;

	IplImage* frame_gray = NULL;
	IplImage* frame_gray_last = NULL;
	IplImage* m_out_image_copy = NULL;
	int nFrmNum = 0;

	//capture = cvCaptureFromCAM(-1); //读摄像头
	capture = cvCaptureFromAVI("diff_flash1s.wmv"); //读视频

	//创建窗口
	cvNamedWindow( "result", 1 );
	cvNamedWindow("差分");

	//逐帧读取视频
	while(frame = cvQueryFrame( capture ))
	{
		nFrmNum++;

		//如果是第一帧，需要申请内存，并初始化
		if(nFrmNum == 1)
		{
			frame_last = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels);
			cvCopy(frame,frame_last);
		}
		else
		{
			frame_gray = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
			frame_gray_last = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);

			m_out_image = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
			m_out_image_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1 );

			cvCvtColor(frame,frame_gray,CV_RGB2GRAY);
			cvCvtColor(frame_last,frame_gray_last,CV_RGB2GRAY);
			cvAbsDiff(frame_gray,frame_gray_last,m_out_image);

			//cvThreshold(m_out_image,m_out_image,128, 255,CV_THRESH_BINARY);

			//将处理后的图像写出到第二个图像中
			if( m_out_image_copy->origin == IPL_ORIGIN_TL )
			{
				cvCopy( m_out_image, m_out_image_copy, 0 );
			}
			else
			{
				cvFlip( m_out_image, m_out_image_copy, 0 );
			}

			frame_last = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels);
			cvCopy(frame,frame_last);

			cvShowImage("差分",m_out_image_copy);
			cvShowImage("result",frame);

			cvReleaseImage( &m_out_image_copy );
			cvReleaseImage( &frame_gray_last );
			cvReleaseImage( &frame_gray );

			if( cvWaitKey( 20 ) >= 0 )
			{
				break;
			}
		}
	}
	cvWaitKey();

	cvReleaseImage( &m_out_image );
	cvReleaseImage( &frame_last );
	cvReleaseImage( &frame );

	cvReleaseCapture( &capture );

	cvDestroyWindow("result");
	cvDestroyWindow("差分");

	return 0;
}
#endif