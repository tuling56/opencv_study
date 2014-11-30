//Function：寻找轮廓并讲解CvSeq的用法
//Source: http://blog.csdn.net/augusdi/article/details/9000276
//Status:
//Improve:
//Info:[11/15/2014 jmy]


#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>
#include <stdio.h>

// #pragma comment(lib, "cv.lib")
// #pragma comment(lib, "cxcore.lib")
// #pragma comment(lib, "highgui.lib")

int contour3( int argc, char** argv )
{
	//声明IplImage指针
	IplImage* pImg = NULL; 
	IplImage* pContourImg = NULL;
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contour = 0;
	int mode = CV_RETR_EXTERNAL;// CV_RETR_EXTERNAL
	unsigned th=0;
	if( argc == 3)
		if(strcmp(argv[2], "all") == 0)
			mode = CV_RETR_CCOMP; //内外轮廓都检测 
	//创建窗口
	cvNamedWindow("src", 1);
	cvNamedWindow("contour",1);
	//载入图像，强制转化为Gray
	if( (pImg = cvLoadImage("samples//square//rect.png", 0)) != 0 )
	{
		cvShowImage( "src", pImg );
		//为轮廓显示图像申请空间
		//3通道图像，以便用彩色显示
		pContourImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,3);
		//copy source image and convert it to BGR image
		cvCvtColor(pImg, pContourImg, CV_GRAY2BGR);
		cvCanny(pImg, pImg, 50, 150, 5); 
		int Num=cvFindContours( pImg, storage, &contour, sizeof(CvContour), mode, CV_CHAIN_APPROX_SIMPLE);
		std::cout<<"The number of Contours is:"<<Num<<std::endl;
		for(;contour!=0;contour=contour->h_next)
		{
			printf("***************************************************\n");
			for(int i=0;i<contour->total;i++)
			{
				CvPoint* p=(CvPoint*)cvGetSeqElem(contour,i);
				printf("p->x=%d,p->y=%d\n",p->x,p->y); 
			}
			//将轮廓画出 
			cvDrawContours(pContourImg, contour, CV_RGB(255,0,0), CV_RGB(0, 255, 0), 0, 2, 0); 
		}
	}
	else
	{
		//销毁窗口
		cvDestroyWindow( "src" );
		cvDestroyWindow( "contour" );
		cvReleaseMemStorage(&storage);
		return -1;
	}
	//显示图像
	cvShowImage( "contour", pContourImg );
	cvWaitKey(0);
	//销毁窗口
	cvDestroyWindow( "src" );
	cvDestroyWindow( "contour" );
	//释放图像
	cvReleaseImage( &pImg ); 
	cvReleaseImage( &pContourImg ); 
	cvReleaseMemStorage(&storage);
	return 0;
}