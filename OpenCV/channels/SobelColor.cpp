/************************************************************************
* Copyright(c) 2014 tuling56
*
* File:	SobelColor.cpp
* Brief: 彩色图像Sobel算子对图像进行边缘检测，（现将彩色图像通道分离，然后再合并）
* Source: http://blog.csdn.net/hustspy1990/article/details/6226367
* Status: 
* Date:	[11/30/2014 jmy]
************************************************************************/

//Sobel算子边缘检测
#include "cv.h"
#include "highgui.h"

int SobelColor_main() 
{

    //定义的变量
	IplImage* pImage= NULL; 
	IplImage* pImgSobelgray= NULL;// 声明IplImage 变量，用于灰度图像Sobel变换
	IplImage* pImg8u= NULL;// 声明IplImage 变量，用于图像格式转换
	IplImage* pImg8uSmooth= NULL;// 声明IplImage 变量，用于存储平滑后的图像
	IplImage* pImgColor= NULL;// 声明IplImage 变量，用于Sobel变换
	IplImage* pImgSobelcolor= NULL;// 声明IplImage 变量，用于彩色图像Sobel变换
    IplImage* pImgPlanes[3] = { 0, 0, 0 };
	//读入图像
	pImage=cvLoadImage("samples/lena.jpg", -1);
	//建立和原始图像一样图像内存区，图像元素的位深度设为IPL_DEPTH_8U，即无符号8位整型
	pImg8u = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U, 1);
	pImg8uSmooth = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U, 1);
  	//对灰度图像进行Sobel变换，将彩色图像转换为灰度图像
	cvCvtColor(pImage, pImg8u, CV_BGR2GRAY);
 	//对图像进行高斯滤波
	cvSmooth( pImg8u, pImg8uSmooth,CV_GAUSSIAN,3,0,0);
	//建立一新图像内存区，图像元素的位深度设为IPL_DEPTH_16S有符号16位整型
	//因为cvSobel函数要求目标图像必须是16-bit图像
	pImgSobelgray = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_16S, 1);
	//计算一阶 x方向的图像差分，可根据需要设置参数
	cvSobel(pImg8uSmooth, pImgSobelgray,0,1,3);
	//将图像格式再转换回来，用于显示
    cvConvertScaleAbs(pImgSobelgray,pImg8u,1,0 ) ;
	

	cvvNamedWindow( "Sobel gray Image", 1 );  
    cvvShowImage( "Sobel gray Image", pImg8u  ); 
	
	//对彩色图像进行Sobel变换，建立3个图像内存区，分别存储图像3个通道，图像元素的位深度设为IPL_DEPTH_8U
	int i;
    for( i = 0; i < 3; i++ )
        pImgPlanes[i] = cvCreateImage( cvSize(pImage ->width, pImage ->height), 8, 1 );
    //建立一新图像内存区，图像元素的位深度设为IPL_DEPTH_16S有符号16位整型
	pImgSobelcolor = cvCreateImage( cvSize(pImage ->width, pImage ->height), IPL_DEPTH_16S, 1 );
     //要求输出图像是 16 位有符号的
    pImgColor = cvCreateImage( cvSize(pImage ->width, pImage ->height), 8, 3 );
    
	//将彩色图像分成3 个单通道图像
    cvCvtPixToPlane(pImage, pImgPlanes[0], pImgPlanes[1], pImgPlanes[2], 0 );
 
	for( i = 0; i < 3; i++ )
    {
        //分别对每通道图像进行Sobel变换
		cvSobel( pImgPlanes[i], pImgSobelcolor,0,1,3 );
		//转化为8位的图像
        cvConvertScaleAbs(pImgSobelcolor, pImgPlanes[i], 1, 0 );   
    }
	//将各通道图像进行合并
	cvCvtPlaneToPix( pImgPlanes[0], pImgPlanes[1], pImgPlanes[2], 0, pImgColor);
   

	cvvNamedWindow( "Sobel color Image", 1 );  
    cvvShowImage( "Sobel color Image", pImgColor);  

	cvWaitKey(0); 

	cvDestroyWindow( " Sobel gray Image " );	
	cvDestroyWindow( " Sobel color Image " );	
	
	//将程序开始定义的变量释放
	cvReleaseImage( & pImage);	
	cvReleaseImage( & pImgSobelgray);
	cvReleaseImage( & pImgSobelcolor);
	cvReleaseImage( & pImg8u);	
	cvReleaseImage( & pImg8uSmooth);
	return 0;
}