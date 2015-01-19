/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	frameMean.cpp
* Brief: 平均值去噪，减去背景
* Source:http://blog.csdn.net/xiaowei_cqu/article/details/7610665
* Status: 
* Date:	[1/15/2015 jmy]
************************************************************************/

#include <cv.h>
#include <highgui.h>



/*
 *	功能：输入视频，取指定帧之间的平均值
 *  参数：
 *  状态:
 */

int framesmeans_main()
{
	CvCapture* capture = cvCaptureFromFile("samples/video/test_o2.mp4");
	IplImage* frame = NULL;
	IplImage * imgsum = NULL;

	int start = 301;
	int end = 304;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, start);

	int count = start;
	while (cvGrabFrame(capture) && count <= end)
	{
		frame = cvRetrieveFrame(capture);// 获取当前帧
		if (imgsum == NULL){
			imgsum = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 3);
			cvZero(imgsum);
		}
		cvAcc(frame, imgsum);

		char testname[100];
		sprintf(testname, "%s%d%s", "image", count, ".jpg");
		cvShowImage(testname, frame);
		cvSaveImage(testname, frame);

		count++;
	}
	IplImage * imgavg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
	cvConvertScale(imgsum, imgavg, 1.0 / 4.0);

	cvShowImage("imageavg", imgavg);
	cvSaveImage("imageavg_4.jpg", imgavg);

	cvWaitKey(0);
	cvReleaseCapture(&capture);
	return 0;
}