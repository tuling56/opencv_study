/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	backgroundRemove.cpp
* Brief: 基于最小二乘法的平面拟合，背景去除
* Source:http://blog.csdn.net/crond123/article/details/3962105
* Status: 效果不是太好，原来亮的地方变暗了
* Date:	[4/6/2015 jmy]
************************************************************************/

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include <algorithm>

//#include "cxerror.h"


/*
*		假定 src mask 为8位单通道 dst 为 [3*1]矩阵
*	    平面方程为 z = ax+by+c;
*		将实验数据带入方程 z 中得到
*         |a|
*		A|b| = b
*		   |c|
*		A = 	(xi,yi,1)  i=1-n n为待拟合点数
*      b  =  (I(xi,yi))  i=1-n n为待拟合点数
*
*      由最小二乘公式的
*
*
*         |a|
*		   |b| = A+b = (A AH)-1 AH b
*		   |c|
*/
int  LeastSquaresFittingPlane(const CvArr* src, const CvArr* mask, CvArr* dst)
{
	CvMat input_hdr, mask_hdr, output_hdr;
	CvMat* input_img, *input_mask, *output_img;
	input_img = cvGetMat(src, &input_hdr);
	input_mask = cvGetMat(mask, &mask_hdr);
	output_img = cvGetMat(dst, &output_hdr);
	if (!CV_ARE_SIZES_EQ(input_img, input_mask))
		return -1;
	int mask_count = 0;
	int width = input_mask->width;
	int height = input_mask->height;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
		if (CV_MAT_ELEM(*input_mask, uchar, y, x) > 0)
			mask_count++;
		}
	CvMat* A = cvCreateMat(mask_count, 3, CV_32FC1);
	CvMat* AH = cvCreateMat(3, mask_count, CV_32FC1);
	CvMat* A_mul_AH = cvCreateMat(3, 3, CV_32FC1);
	CvMat*  AH_mul_b = cvCreateMat(3, 1, CV_32FC1);
	CvMat* b = cvCreateMat(mask_count, 1, CV_32FC1);
	//	CvMat* result =  cvCreateMat(3,1,CV_32SC1);
	int mask_index = 0;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
		uchar cur_val = CV_MAT_ELEM(*input_mask, uchar, y, x);
		if (cur_val == 0)
			continue;
		CV_MAT_ELEM(*A, float, mask_index, 0) = (float)x;
		CV_MAT_ELEM(*A, float, mask_index, 1) = (float)y;
		CV_MAT_ELEM(*A, float, mask_index, 2) = (float)1;
		CV_MAT_ELEM(*b, float, mask_index, 0) = (float)CV_MAT_ELEM(*input_img, uchar, y, x);
		mask_index++;
		}
	cvTranspose(A, AH); // AH = A的转制 
	cvMulTransposed(A, A_mul_AH, 1);//A_mul_AH = A AH
	cvInvert(A_mul_AH, A_mul_AH);// A_mul_AH = (A_mul_AH)-1 
	cvmMul(AH, b, AH_mul_b);//AH_mul_b = AH b
	cvmMul(A_mul_AH, AH_mul_b, dst);//dst = (a,b,c)H = A_mul_AH AH_mul_b
	cvReleaseMat(&A);
	cvReleaseMat(&AH);
	cvReleaseMat(&A_mul_AH);
	cvReleaseMat(&AH_mul_b);
	cvReleaseMat(&b);
	return 0;
}
void backgroundCorrectLeastSquaresFittingPlane(IplImage * src, IplImage * dst, IplImage * bgplane)
{
	IplImage * mask = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvSet(mask, cvScalar(255));
	CvMat * result = cvCreateMat(3, 1, CV_32FC1);
	LeastSquaresFittingPlane(src, mask, result);
	float a = CV_MAT_ELEM(*result, float, 0, 0);
	float b = CV_MAT_ELEM(*result, float, 1, 0);
	float c = CV_MAT_ELEM(*result, float, 2, 0);
	for (int y = 0; y < src->height; y++)
		for (int x = 0; x < src->width; x++)
		{
		uchar temp = (uchar)(a*x + b*y + c);
		if (bgplane != NULL)
			CV_IMAGE_ELEM(bgplane, uchar, y, x) = temp;
		CV_IMAGE_ELEM(dst, uchar, y, x) = cv::max((CV_IMAGE_ELEM(src, uchar, y, x) - temp), 0);
		}
	cvReleaseImage(&mask);
	cvReleaseMat(&result);
}

int main(int argc, char* argv[])
{
	IplImage* pImg = cvLoadImage("E:\\360YPan\\samples\\rice.png", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage * plane = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	IplImage * dst = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	
	backgroundCorrectLeastSquaresFittingPlane(pImg, dst, plane);

	cvSaveImage("result.jpg", dst);
	cvNamedWindow("src", 1);
	cvShowImage("src", pImg);
	cvNamedWindow("dst", 1);
	cvShowImage("dst", dst);
	cvWaitKey(0); 
	cvDestroyWindow("src");
	cvReleaseImage(&pImg); 
	
	return 0;
}
