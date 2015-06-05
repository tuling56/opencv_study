/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	maxEntropy.cpp
* Brief: 最大熵阈值分割
* Source:http://blog.csdn.net/crond123/article/details/3952597
* Status: 
* Date:	[4/6/2015 jmy]
************************************************************************/
#include "cv.h"
#include "highgui.h"

int HistogramBins = 256;
float HistogramRange1[2] = { 0, 255 };
float *HistogramRange[1] = { &HistogramRange1[0] };
typedef
enum { back, object } entropy_state;

double caculateCurrentEntropy(CvHistogram * Histogram1, int cur_threshold, entropy_state state)
{
	int start, end;
	if (state == back)	{
		start = 0; end = cur_threshold;
	}
	else	{
		start = cur_threshold; end = 256;
	}
	int  total = 0;
	for (int i = start; i < end; i++)	{
		total += (int)cvQueryHistValue_1D(Histogram1, i);
	}
	double cur_entropy = 0.0;
	for (int i = start; i < end; i++){
		if ((int)cvQueryHistValue_1D(Histogram1, i) == 0)
			continue;
		double percentage = cvQueryHistValue_1D(Histogram1, i) / total;
		cur_entropy += -percentage*logf(percentage);
	}
	return cur_entropy;
}

void  MaxEntropy(IplImage *src, IplImage *dst)
{
	assert(src != NULL);
	assert(src->depth == 8 && dst->depth == 8);
	assert(src->nChannels == 1);
	CvHistogram * hist = cvCreateHist(1, &HistogramBins, CV_HIST_ARRAY, HistogramRange);
	cvCalcHist(&src, hist);
	double maxentropy = -1.0;
	int max_index = -1;
	for (int i = 0; i < HistogramBins; i++)	{
		double cur_entropy =
			caculateCurrentEntropy(hist, i, object) + caculateCurrentEntropy(hist, i, back);
		if (cur_entropy > maxentropy){
			maxentropy = cur_entropy;
			max_index = i;
		}
	}
	cvThreshold(src, dst, (double)max_index, 255, CV_THRESH_BINARY);
	cvReleaseHist(&hist);
}

//功能测试区
//int main(int argc, char* argv[])
//{
//	IplImage* src = cvLoadImage("samples\\segment\\1.png", 0);
//	if (src == NULL){
//		printf("加载图片失败");
//		return -1;
//	}
//
//	cvNamedWindow("Image", 1);
//	IplImage* dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
//	MaxEntropy(src, dst);
//	cvShowImage("Image", dst);
//	cvWaitKey(0);
//	cvDestroyWindow("Image");
//	cvReleaseImage(&src);
//	cvReleaseImage(&dst);
//
//	return 0;
//}