//图像的轮廓检测上
//By MoreWindows (http://blog.csdn.net/MoreWindows)
#include <opencv2/opencv.hpp>
using namespace std;

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


int contour1()
{	
	const char *pstrWindowsSrcTitle = "原图(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsOutLineTitle = "轮廓图(http://blog.csdn.net/MoreWindows)";

	const int IMAGE_WIDTH = 400;
	const int IMAGE_HEIGHT = 200;

	// 创建图像
	IplImage *pSrcImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	// 填充成白色
	cvRectangle(pSrcImage, cvPoint(0, 0), cvPoint(pSrcImage->width, pSrcImage->height), CV_RGB(255, 255, 255), CV_FILLED);
	
	// 画圆
	CvPoint ptCircleCenter = cvPoint(IMAGE_WIDTH / 4, IMAGE_HEIGHT / 2);
	int nRadius = 80;
	cvCircle(pSrcImage, ptCircleCenter, nRadius, CV_RGB(255, 255, 0), CV_FILLED);
	ptCircleCenter = cvPoint(IMAGE_WIDTH / 4, IMAGE_HEIGHT / 2);
	nRadius = 30;
	cvCircle(pSrcImage, ptCircleCenter, nRadius, CV_RGB(255, 255, 255), CV_FILLED);
	
	// 画矩形
	CvPoint ptLeftTop = cvPoint(IMAGE_WIDTH / 2 + 20, 20);
	CvPoint ptRightBottom = cvPoint(IMAGE_WIDTH - 20, IMAGE_HEIGHT - 20);
	cvRectangle(pSrcImage, ptLeftTop, ptRightBottom, CV_RGB(0, 255, 255), CV_FILLED);
	ptLeftTop = cvPoint(IMAGE_WIDTH / 2 + 60, 40);
	ptRightBottom = cvPoint(IMAGE_WIDTH - 60, IMAGE_HEIGHT - 40);
	cvRectangle(pSrcImage, ptLeftTop, ptRightBottom, CV_RGB(255, 255, 255), CV_FILLED);
	
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
	int nLevels = 5;
	// 填充成白色
	cvRectangle(pOutlineImage, cvPoint(0, 0), cvPoint(pOutlineImage->width, pOutlineImage->height), CV_RGB(255, 255, 255), CV_FILLED);
	/*要修改的也正是这个地方*/
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