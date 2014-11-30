//Function：图像的Canny边缘检测
//Source:By MoreWindows (http://blog.csdn.net/morewindows/article/details/8239625)
//Status:
//Improve:
//Info:[11/18/2014 jmy]

#include <opencv2/opencv.hpp>

using namespace std;

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
IplImage *g1_pSrcImage, *g_pCannyImg;
const char *pstrWindowsCannyTitle = "边缘检测图(canny_trackbar)";


//cvCreateTrackbar的回调函数
void on_trackbar(int threshold)
{
	//canny边缘检测
	printf("threshold:%d\n",threshold);
	cvCanny(g1_pSrcImage, g_pCannyImg, threshold, threshold * 3, 3);
	cvShowImage(pstrWindowsCannyTitle, g_pCannyImg);
}

int canny_trackbar(char*filename)
{
	//const char *pstrImageName = "lena.jpg";
	const char *pstrWindowsSrcTitle = "原图(canny_trackbar)";
	const char *pstrWindowsToolBar = "Threshold";

	//从文件中载入图像的灰度图CV_LOAD_IMAGE_GRAYSCALE - 灰度图
	g1_pSrcImage = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
	g_pCannyImg = cvCreateImage(cvGetSize(g1_pSrcImage), IPL_DEPTH_8U, 1);

	//创建窗口
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(pstrWindowsCannyTitle, CV_WINDOW_AUTOSIZE);


	//创建滑动条
	int nThresholdEdge = 1;
	cvCreateTrackbar(pstrWindowsToolBar, pstrWindowsCannyTitle, &nThresholdEdge, 100, on_trackbar);

	//在指定窗口中显示图像
	cvShowImage(pstrWindowsSrcTitle, g1_pSrcImage);
	on_trackbar(1);

	//等待按键事件
	cvWaitKey();

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(pstrWindowsCannyTitle);
	cvReleaseImage(&g1_pSrcImage);
	cvReleaseImage(&g_pCannyImg);


	return 0;
}

