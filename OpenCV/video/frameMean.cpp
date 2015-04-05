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
#include <iostream>

using std::cout;
using std::endl;



/*
 *	功能：输入视频，取指定帧之间的平均值
 *  参数：
 *  状态:
 */
int frameMean()
{
	CvCapture* capture = cvCaptureFromFile( "samples/video/test_o2.mp4");//"test.avi");//
	IplImage* frame = NULL;
	IplImage* framegray = NULL;
	IplImage * framesum = NULL;
	cvNamedWindow("diff", 1);

	int start = 0, curframe = 0, meannum = 6;
	while (cvGrabFrame(capture)) 
	{
		curframe = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
		start = (curframe - meannum > 0) ? (curframe - meannum) : 0;
		cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, start);
		frame = cvRetrieveFrame(capture);// 获取当前帧
		framegray= cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, framegray, CV_BGR2GRAY);
		cout << "==================================" << endl;
		cout << "当前帧：" << curframe << endl
			 << "取平均的帧开始号: " << start << endl;
		int countstart = start;
		while (start<curframe)
		{
			cout << "开始累积" << start << endl;
			frame = cvRetrieveFrame(capture);// 获取当前帧
			cvCvtColor(frame, framegray, CV_BGR2GRAY);
			if (framesum == NULL){
				framesum = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);
				cvZero(framesum);
			}
			cvAcc(framegray, framesum);

			//char testname[100];
			//sprintf(testname, "%s%d%s", "image", start, ".jpg");
			//cvShowImage(testname, frame);
			//cvSaveImage(testname, frame);

			cvGrabFrame(capture);
			start++;
		}
		cout << "累积完毕，开始计算平均" << curframe << "-[" << countstart << "---" << curframe - 1 <<"]"<< endl;
		IplImage * frameavg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		if (curframe>meannum){
			cvConvertScale(framesum, frameavg, 1.0 / meannum);
		}
		else{
			cvConvertScale(framesum, frameavg, 1.0 / curframe);
		}

		cvAbsDiff(framegray,frameavg,framegray);
		//cvThreshold(framegray,framegray,155,255,CV_THRESH_BINARY);
		cvShowImage("diff", framegray);
		//cvSaveImage("diff_4.jpg", frame);
		cvReleaseImage(&frameavg);
		cvReleaseImage(&framegray);
		
		char c = cvWaitKey(10);
		if (c==27)	{
			break;
		}
		
	}
	
	cvReleaseCapture(&capture);
	cvDestroyWindow("diff");
	return 0;
}

//功能测试区
