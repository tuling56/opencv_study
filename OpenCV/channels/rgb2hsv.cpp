/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	rgb2hsv.cpp
* Brief: opencv基于HSV的肤色分割,关键在于学习色彩空间转换
* Source:http://www.cnblogs.com/xingma0910/archive/2013/07/18/3197502.html
* Status: 
* Date:	[1/10/2015 jmy]
************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


/*
 *	功能：在HSV颜色空间对图像进行肤色模型分割
 *  输入：src-待处理的图像
 *  输出：imgout-输出图像
 *  状态：
 */
IplImage* SkinSegmentHSV(IplImage* src,IplImage* imgout)
{
	//定义一些中间指针，指向处理过程中的中间变量
	IplImage* HSV = NULL;  
	IplImage* HImg= NULL;
	IplImage* SImg= NULL;
	IplImage* VImg= NULL;
	IplImage* result = NULL;//指向处理后的结果

	if (!src||!imgout)  {   return NULL;  }

	//获取输入图像的大小
	CvSize SrcSize = cvGetSize(src);

	//为中间结果指针分配存储空间
	HSV = cvCreateImage(SrcSize,8,3);
	HImg= cvCreateImage(SrcSize,8,1);  
	SImg= cvCreateImage(SrcSize,8,1);  
	VImg= cvCreateImage(SrcSize,8,1);  

	result= cvCreateImage(cvGetSize(imgout),8,1);//还是默认的BGR

	//将图像从RGB颜色空间转换到HSV空间
	cvCvtColor(src,HSV,CV_BGR2HSV);
	//将HSV分解为三张单通道图像，便于后面就行处理  
	cvSplit(HSV,HImg,SImg,VImg,NULL);
	
	int i, j;  int value = 0;
	//对通道H进行处理，色相 
	for (i = 0; i < HImg->height; i++)
	{  
		for (j = 0; j < HImg->width; j++)
     	{   
			value = cvGetReal2D(HImg,i,j);   
			if (value >=0 && value <= 25){     
				 *(HImg->imageData+i*HImg->widthStep+j) = 255;  //二值化 
			}    
			else {     
				*(HImg->imageData+i*HImg->widthStep+j) = 0;    
			}               
		}  
	}

	//对通道S进行处理，饱和度
	for (i = 0; i < SImg->height; i++)  
	{   
		for (j = 0; j < SImg->width; j++)
	   {    
			value = cvGetReal2D(SImg, i, j);
			if (value >= 26 && value <= 200) {
				*(SImg->imageData + i*SImg->widthStep + j) = 255;
			}
			else {
				*(SImg->imageData + i*SImg->widthStep + j) = 0;
			}
	   }
	}
	//对通道V进行处理，亮度 
	for (i = 0; i < VImg->height; i++)  
	{  
		for (j = 0; j < VImg->width; j++)   
		{   
			value = cvGetReal2D(VImg,i,j);    
			if (value >=20 && value <= 200)	{     
				*(VImg->imageData+i*VImg->widthStep+j) = 255;     
			}     
			else {    
				*(VImg->imageData+i*VImg->widthStep+j) = 0;     
			}    
		}   
	}

	//HSV相与得到最后的结果？
	cvAnd(HImg,SImg,result,0);  
	cvAnd(VImg,result,result,0); 

	//对所得到的结果进行形态学腐蚀处理，去除小区域部分
	cvErode(result,result);  
	cvErode(result,result); 
	cvErode(result,result);  //腐蚀三次，膨胀一次
	cvDilate(result,result);

	//将处理后的结果赋值给输出图像
	cvCopy(result,imgout);  

	//释放相关资源  
	cvReleaseImage(&src);  
	cvReleaseImage(&HSV);  
	cvReleaseImage(&HImg); 
	cvReleaseImage(&SImg); 
	cvReleaseImage(&VImg);  
	cvReleaseImage(&result);

	//返回处理后的结果
	return imgout;

}

//功能测试区
//int main()
//{
//	IplImage*src=cvLoadImage("samples/hsv.png");
//	IplImage*output=cvCreateImage(cvGetSize(src),src->depth,1);
//	//cvCvtColor(output,output,CV_BGR2HSV);
//	SkinSegmentHSV(src,output);
//	waitKey();
//}