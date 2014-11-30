//图像的轮廓检测下
//By MoreWindows (http://blog.csdn.net/MoreWindows)
#include <opencv2/opencv.hpp>
using namespace std;


IplImage *greyimg = NULL;
const char *binWndName = "二值图(http://blog.csdn.net/MoreWindows)";
const char *contourWndName = "轮廓图(http://blog.csdn.net/MoreWindows)";
CvSeq *g_pcvSeq = NULL;

void contour2_trackbar(int pos)
{
	// 转为二值图
	IplImage *pBinaryImage = cvCreateImage(cvGetSize(greyimg), IPL_DEPTH_8U, 1);
	cvThreshold(greyimg, pBinaryImage, pos, 255, CV_THRESH_BINARY);
	// 显示二值图
	cvShowImage(binWndName, pBinaryImage);

	CvMemStorage* cvMStorage = cvCreateMemStorage();
	// 检索轮廓并返回检测到的轮廓的个数
	cvFindContours(pBinaryImage,cvMStorage, &g_pcvSeq);

	IplImage *pOutlineImage = cvCreateImage(cvGetSize(greyimg), IPL_DEPTH_8U, 3);
	int _levels = 5;
	cvZero(pOutlineImage);
	cvDrawContours(pOutlineImage, g_pcvSeq, CV_RGB(255,0,0), CV_RGB(0,255,0), _levels);
	cvShowImage(contourWndName, pOutlineImage);

	cvReleaseMemStorage(&cvMStorage);
	cvReleaseImage(&pBinaryImage);
	cvReleaseImage(&pOutlineImage);
}

int contour2(char*path)
{	
	const char *pstrWindowsSrcTitle = "原图(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsToolBarName = "二值化";

	// 从文件中加载原图
	IplImage *pSrcImage = cvLoadImage(path, CV_LOAD_IMAGE_UNCHANGED);
	if (!pSrcImage)
	{
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
	cvCreateTrackbar(pstrWindowsToolBarName, binWndName, &nThreshold, 254, contour2_trackbar);

	contour2_trackbar(1);

	cvWaitKey(0);

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(binWndName);
	cvDestroyWindow(contourWndName);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&greyimg);
	return 0;
}