#include <iostream>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

using namespace std;

int templateMatch()
{
	IplImage *src = cvLoadImage("src.jpg", 0);
	IplImage *srcResult = cvLoadImage("src.jpg", 3);  //用来显示
	IplImage *templat = cvLoadImage("template.jpg", 0);
	IplImage *result;
	if(!src || !templat)
	{
		cout << "打开图像失败"<< endl;
		return 0;
	}
	int srcW, srcH, templatW, templatH, resultH, resultW;
	srcW = src->width;
	srcH = src->height;
	templatW = templat->width;
	templatH = templat->height;
	if(srcW < templatW || srcH < templatH)
	{
		cout <<"模板不能比原图像小" << endl;
		return 0;
	}
	resultW = srcW - templatW + 1;
	resultH = srcH - templatH + 1;
	result = cvCreateImage(cvSize(resultW, resultH), 32, 1);    
	cvMatchTemplate(src, templat, result, CV_TM_SQDIFF);   
	double minValue, maxValue;
	CvPoint minLoc, maxLoc;
	cvMinMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);  
	cvRectangle(srcResult, minLoc, cvPoint(minLoc.x + templatW, minLoc.y+ templatH), cvScalar(0,0,255));
	
	cvNamedWindow("srcResult", 0);
	cvNamedWindow("templat", 0);
	cvShowImage("srcResult", srcResult);
	cvShowImage("templat", templat);
	
	cvWaitKey(0);
	cvReleaseImage(&result);
	cvReleaseImage(&templat);
	cvReleaseImage(&srcResult);
	cvReleaseImage(&src);
	return 0;
}