#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/legacy/compat.hpp>
#include <opencv2/imgproc/imgproc_c.h>

using namespace cv;

IplImage *DrawHistogram(CvHistogram*hist, float scaleX = 1, float scaleY = 1)
{  // 画直方图   
	float histMax = 0;  
	cvGetMinMaxHistValue(hist, 0 , &histMax, 0, 0);  // 取得直方图中的最值   
	IplImage *imgHist = cvCreateImage(cvSize(256 * scaleX, 64*scaleY), 8, 1);
	cvZero(imgHist); //// 清空随机值
	for(int i = 0; i < 255; i++)
	{
		float histValue = cvQueryHistValue_1D(hist, i); // 取得直方图中的i值
		float nextValue = cvQueryHistValue_1D(hist, i+1);
		int numPt = 5;
		CvPoint pt[5];
		pt[0] = cvPoint(i*scaleX, 64*scaleY);
		pt[1] = cvPoint((i+1)*scaleX, 64*scaleY);
		pt[2] = cvPoint((i+1)*scaleX, (1 -(nextValue/histMax))* 64 * scaleY);
		pt[3] = cvPoint((i+1)*scaleX, (1 -(histValue/histMax))* 64 * scaleY);
		pt[4] = cvPoint(i*scaleX, 64*scaleY);
		cvFillConvexPoly(imgHist, pt, numPt, cvScalarAll(255));
	}
	return imgHist;
}  


void calcDrawHist(char* path)
{
	IplImage* img = cvLoadImage(path,1); //RGB加载
	if(!img)
		return;
	//然后，创建并初始化一个直方图：
	int numBins = 256;
	float range[] = {0, 255};
	float *ranges[] = { range };

	CvHistogram *hist = cvCreateHist(1, &numBins, CV_HIST_ARRAY, ranges, 1);
	cvClearHist(hist);
	//在这里，我们使用的是1维的直方图，并且该直方图有256个直方柱。范围是0-255，函数cvCreateHist会自动将这个范围分解成256个直方柱。

	//然后，将加载的彩色图像的BGR三个通道进行分解：分配图像空间，并调用函数cvSplit进行通道的分解：
	IplImage* imgRed = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* imgGreen = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* imgBlue = cvCreateImage(cvGetSize(img), 8, 1);

	cvSplit(img, imgBlue, imgGreen, imgRed, NULL);
	//cvSplit的参数是按照B，G，R这样的一个顺序的。图像在内存中的存储也是按照B，G，R的顺序进行存储。

	
	//计算亮度直方图（灰度直方图）
	IplImage*grayimg=cvCreateImage(cvGetSize(img),img->depth,1);
	cvCvtColor(img,grayimg,CV_BGR2GRAY);
	cvCalcHist(&grayimg, hist, 0, 0);
	IplImage* imgHistGray = DrawHistogram(hist);
	cvClearHist(hist);
	
	
	
	
	//（2）计算直方图，显示直方图
	//使用R通道图像，计算R通道图像的直方图：
	cvCalcHist(&imgRed, hist, 0, 0);
	IplImage* imgHistRed = DrawHistogram(hist);
	cvClearHist(hist);
	///函数cvCalcHist计算imgRed的直方图，并将直方图存储到hist中。imgHistRed是直方图的hist视觉效果。函数DrawHistogram将直方图绘画到图像imgHistRed上，稍后介绍这个函数。

	//对其他两个通道，也进行相同的操作：
	cvCalcHist(&imgGreen, hist, 0, 0);
	IplImage* imgHistGreen = DrawHistogram(hist);
	cvClearHist(hist);

	cvCalcHist(&imgBlue, hist, 0, 0);
	IplImage* imgHistBlue = DrawHistogram(hist);
	cvClearHist(hist);
	
	//最后，在窗口中显示这些直方图：
	cvNamedWindow("Gray");
	cvNamedWindow("Red");
	cvNamedWindow("Green");
	cvNamedWindow("Blue");

	cvShowImage("Gray",imgHistGray);
	cvShowImage("Red", imgHistRed);
	cvShowImage("Green", imgHistGreen);
	cvShowImage("Blue", imgHistBlue);

	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&grayimg);
	cvReleaseImage(&imgHistGray);
	cvReleaseImage(&imgHistRed);
	cvReleaseImage(&imgHistGreen);
	cvReleaseImage(&imgHistBlue);
	cvDestroyAllWindows(); 
}