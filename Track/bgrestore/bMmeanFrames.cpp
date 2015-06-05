/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	move_dete_roi.cpp
* Brief: 帧平均的方法进行背景建模
* Reference:
* Status: 
* Date:	[5/6/2015 jmy]
************************************************************************/
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
 
int bMmeanFrames(char*videoname)
{
  IplImage* pFrame = NULL; 
  IplImage* pFrImg = NULL;
  IplImage* pBkImg = NULL;
  IplImage* pROIImg = NULL;

  CvMat* pFrameMat = NULL;
  CvMat* pFrMat = NULL;
  CvMat* pBkMat = NULL;
  CvCapture* pCapture = NULL;
  int nFrmNum = 0;

  //创建窗口
  cvNamedWindow("video", 1);
  cvNamedWindow("background",1);
  cvNamedWindow("foreground",1);
  cvNamedWindow("ROI");  


  //使窗口有序排列
  cvMoveWindow("video", 30, 0);
  cvMoveWindow("background", 360, 0);
  cvMoveWindow("foreground", 690, 0);

  /*关于视频选择的部分,当用摄像头时稍加修改*/
  
  /*
  if( argc > 2 ){
      fprintf(stderr, "Usage: 程序名称 [video_file_name]\n");
      return -1;
    }

  //打开摄像头
  if (argc ==1)
    if( !(pCapture = cvCaptureFromCAM(-1))){
	    fprintf(stderr, "Can not open camera.\n");
        return -2;
      }
 */


  //读取视频文件
 // if(argc == 2)
    if( !(pCapture = cvCaptureFromFile(videoname))){
        fprintf(stderr, "Can not open video file %s\n", videoname);
        return -2;
      }

  //逐帧读取视频
  while(pFrame = cvQueryFrame( pCapture ))
    {
      nFrmNum++;
      if(nFrmNum == 1) //如果是第一帧，需要申请内存，并初始化
     {
		 pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1);
		 pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1);	 

		 pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
		 pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
		 pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);

		 //转化成单通道图像再处理，注意第一帧设置前景和背景都是第一帧图像
		 cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
		 cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);


		 //转化成Mat矩阵进行处理
		 cvConvert(pFrImg, pFrameMat);
		 cvConvert(pFrImg, pFrMat);
		 cvConvert(pFrImg, pBkMat);

    }

      else
    {
		 
		 cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
		 cvConvert(pFrImg, pFrameMat);

		 //高斯滤波先，以平滑图像
		 cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

		 //当前帧跟背景图相减的前景图像
		 cvAbsDiff(pFrameMat, pBkMat, pFrMat);

		 //二值化前景图
		 cvThreshold(pFrMat, pFrImg, 60, 255.0, CV_THRESH_BINARY);

		 //（前景图）进行形态学滤波，去掉噪音 
		 cvErode(pFrImg, pFrImg, 0, 1);
		 cvDilate(pFrImg, pFrImg, 0, 1);

		 //更新背景
		 cvRunningAvg(pFrameMat, pBkMat, 0.03, 0);
		 
		 //将背景转化为图像格式，用以显示
		 cvConvert(pBkMat, pBkImg);
	
             //ROI处理部分
			 pROIImg = cvCreateImage(cvGetSize(pBkImg),IPL_DEPTH_8U, 1);  
			 cvCopy(pBkImg, pROIImg); 
			 cvRectangle(pFrame,cvPoint(30,381),cvPoint(523,433),CV_RGB(255,0,0),2);

			 CvRect omega = cvRect(30, 381, 495, 53);//算法在于取得自动参数，而不是手工设定的  
			 cvSetImageROI(pROIImg, omega); //是在新copy的图像上进行操作而不是在原始图像上 


			 //在新图像上进行二值化
			cvThreshold(pROIImg,pROIImg,80,255,CV_THRESH_BINARY);

 
		 //显示图像
		 cvShowImage("video", pFrame);
		 cvShowImage("background", pBkImg);
		 cvShowImage("foreground", pFrImg);
		 cvShowImage("ROI", pROIImg); 


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
  cvDestroyWindow("ROI");

  //释放图像和矩阵
  cvReleaseImage(&pFrImg);
  cvReleaseImage(&pBkImg);
  cvReleaseImage(&pROIImg);

  cvReleaseMat(&pFrameMat);
  cvReleaseMat(&pFrMat);
  cvReleaseMat(&pBkMat);

  cvReleaseCapture(&pCapture);

  return 0;

}


/*
 *	识别时机判定,指定区域截图
 * 设置了图像的ROI后，以后所有的操作都是对图像的ROI区域进行操作。
 */

//c++读取txt文件，然后把每个逗号之间的数字存储起来（任务），把位置参数传给ROI
