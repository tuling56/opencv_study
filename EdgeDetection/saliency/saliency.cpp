/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	saliency.cpp
* Brief: 视觉显著性检测，OpenCV 2.1 测试例程
* Source:http://www.cnblogs.com/CCBB/archive/2011/05/19/2051442.html
* Status: 可用
* Date:	[3/29/2015 jmy]
************************************************************************/

#include "highgui.h"
#include "cv.h"
#include <string>

//所有的以新风格命名的函数都在 cv 命名空间中
//如果希望不要每次都输入 cv:: ，则可使用下面语句
using namespace cv;
using namespace std;

//傅里叶正变换
void fft2(IplImage *src, IplImage *dst);

int main_saliency()
{
	string imagePath = "samples//scene//111-1141_IMG.JPG";

	//////////////////////////////////////////////////////////////////////////
	//显著性计算
	//参考论文：Saliency Detection: A Spectral Residual Approach

	//amplitude和phase分别是图像的振幅谱和相位谱
	IplImage *src, *ImageRe, *ImageIm, *Fourier, *Inverse, *LogAmplitude, *Sine, *Cosine;
	IplImage *Saliency, *Residual;
	IplImage *tmp1, *tmp2, *tmp3;
	double minNum = 0, maxNum = 0, scale, shift;
	int i, j, nRow, nCol;

	//加载源图像，第二个参数为0表示将输入的图片转为单通道，大于0为三通道
	src = cvLoadImage(imagePath.c_str(), 0);
	//注意Fourier是一个两通道的图像，一个通道为实部，一个为虚部
	Fourier = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);
	Inverse = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);
	//频谱的实部
	ImageRe = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
	//频谱的虚部
	ImageIm = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
	//log振幅谱
	LogAmplitude = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
	//正弦谱
	Sine = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
	//余弦谱
	Cosine = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);

	//频谱冗余（spectral residual）
	Residual = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
	//特征map(Saliency map)
	Saliency = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

	//临时的空间
	tmp1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
	tmp2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
	tmp3 = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);

	nRow = src->height;
	nCol = src->width;

	//归一化一下
	scale = 1.0 / 255.0;
	cvConvertScale(src, tmp1, scale, 0);

	//傅里叶变换，得到的Fourier有两个通道，一个是实部，一个是虚部
	fft2(tmp1, Fourier);

	//将傅里叶谱的实部和虚部存放到对应的图像中去。
	cvSplit(Fourier, ImageRe, ImageIm, 0, 0);

	//计算傅里叶振幅谱，实部和虚部平方和再开方，得到振幅谱存到tmp3中
	cvPow(ImageRe, tmp1, 2.0);
	cvPow(ImageIm, tmp2, 2.0);
	cvAdd(tmp1, tmp2, tmp3);
	cvPow(tmp3, tmp3, 0.5);

	//计算正弦谱和余弦谱和自然对数谱
	cvLog(tmp3, LogAmplitude);
	cvDiv(ImageIm, tmp3, Sine);
	cvDiv(ImageRe, tmp3, Cosine);

	//对LogAmplitude做3*3均值滤波
	cvSmooth(LogAmplitude, tmp3, CV_BLUR, 3, 3);

	//计算出剩余普
	cvSub(LogAmplitude, tmp3, Residual);

	/************************************************************************/
	/*
	对exp(Residual+i*Phase)作傅里叶反变换
	由欧拉公式：
	exp(r+i*Θ) = exp(r)*(cos(Θ) + i*sin(Θ)) = exp(r)*cos(Θ) + i*exp(r)*sin(Θ)
	其中Θ是相位。

	而sin(Θ) = ImageIm/Amplitude; cos(Θ) = ImageRe/Amplitude;
	*/
	/************************************************************************/
	cvExp(Residual, Residual);
	cvMul(Residual, Cosine, tmp1);
	cvMul(Residual, Sine, tmp2);

	//将剩余普Residual作为实部，相位谱Phase作为虚部
	cvMerge(tmp1, tmp2, 0, 0, Fourier);

	//实现傅里叶逆变换
	cvDFT(Fourier, Inverse, CV_DXT_INV_SCALE);

	cvSplit(Inverse, tmp1, tmp2, 0, 0);

	//求出对应的实部虚部平方和
	cvPow(tmp1, tmp1, 2);
	cvPow(tmp2, tmp2, 2);
	cvAdd(tmp1, tmp2, tmp3, NULL);

	//高斯滤波
	cvSmooth(tmp3, tmp3, CV_GAUSSIAN, 7, 7);

	cvMinMaxLoc(tmp3, &minNum, &maxNum, NULL, NULL);
	scale = 255 / (maxNum - minNum);
	shift = -minNum * scale;

	//将shift加在ImageRe各元素按比例缩放的结果上，存储为ImageDst
	cvConvertScale(tmp3, Saliency, scale, shift);
	//二值化（主要方便观察效果）
	//cvThreshold(Saliency, Saliency,30,255,THRESH_BINARY);

	cvNamedWindow("Saliency", 1);
	cvShowImage("Saliency", Saliency);

	cvWaitKey(0);

	//释放图像
	cvReleaseImage(&src);
	cvReleaseImage(&ImageIm);
	cvReleaseImage(&ImageRe);
	cvReleaseImage(&Fourier);
	cvReleaseImage(&Inverse);
	cvReleaseImage(&LogAmplitude);
	cvReleaseImage(&Sine);
	cvReleaseImage(&Cosine);
	cvReleaseImage(&Saliency);
	cvReleaseImage(&Residual);
	cvReleaseImage(&tmp1);
	cvReleaseImage(&tmp2);
	cvReleaseImage(&tmp3);

	cvDestroyAllWindows();

	return 0;
}

/**************************************************************************
//src IPL_DEPTH_8U
//dst IPL_DEPTH_64F
**************************************************************************/
//傅里叶正变换
void fft2(IplImage *src, IplImage *dst)
{   //实部、虚部
	IplImage *image_Re = 0, *image_Im = 0, *Fourier = 0;
	//   int i, j;
	image_Re = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);  //实部
	//Imaginary part
	image_Im = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);  //虚部
	//2 channels (image_Re, image_Im)
	Fourier = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);

	/************************************************************************/
	/*
	void cvConvertScale( const CvArr* src, CvArr* dst, double scale=1, double shift=0 );
	src
	原数组.
	dst
	输出数组
	scale
	比例因子.
	shift
	原数组元素按比例缩放后添加的值。

	函数 cvConvertScale 有多个不同的目的因此就有多个意义，
	函数按比例从一个数组中拷贝元素到另一个元素这种操作是最先执行的，
	或者任意的类型转换，正如下面的操作：

	dst(I)=src(I)*scale + (shift,shift,...)

	多通道的数组对各个地区通道是独立处理的。
	*/
	/************************************************************************/

	//实部的值初始设为源图像，虚部的值初始设为0
	// Real part conversion from u8 to 64f (double)
	cvConvertScale(src, image_Re, 1, 0);
	// Imaginary part (zeros)
	cvZero(image_Im);
	// Join real and imaginary parts and stock them in Fourier image
	cvMerge(image_Re, image_Im, 0, 0, Fourier);
	// Application of the forward Fourier transform
	cvDFT(Fourier, dst, CV_DXT_FORWARD);
	cvReleaseImage(&image_Re);
	cvReleaseImage(&image_Im);
	cvReleaseImage(&Fourier);
}