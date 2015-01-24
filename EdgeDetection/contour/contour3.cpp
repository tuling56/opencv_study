//Function：寻找轮廓并讲解CvSeq的用法
//Source: http://blog.csdn.net/augusdi/article/details/9000276
//Status:基于OpenCV1
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

	cvNamedWindow("src", 1);
	cvNamedWindow("contour",1);
	
	//以灰度图像载入，然后强制将灰度图转化为彩色图
	if( (pImg = cvLoadImage("samples//200.png", 0)) != 0 )
	{
		cvShowImage( "src", pImg );
		//为轮廓显示图像申请空间，3通道图像，以便用彩色显示
		pContourImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,3);
		//copy source image and convert it to BGR image
		cvCvtColor(pImg, pContourImg, CV_GRAY2BGR);//只不过是将每个通道都赋成相同的灰度值
		cvCanny(pImg, pImg, 50, 150, 5); //在边缘检测图像的基础上再寻找轮廓
		int Num=cvFindContours( pImg, storage, &contour, sizeof(CvContour), mode, CV_CHAIN_APPROX_SIMPLE);
		std::cout<<"The number of Contours is:"<<Num<<std::endl;
		for(;contour!=0;contour=contour->h_next)
		{
			printf("***************************************************\n");
			for(int i=0;i<contour->total;i++)
			{
				CvPoint* p=(CvPoint*)cvGetSeqElem(contour,i);//依次取得轮廓中的每个点
				printf("p->x=%d,p->y=%d\n",p->x,p->y); 
			}
			//将轮廓画出 
			cvDrawContours(pContourImg, contour, CV_RGB(255,0,0), CV_RGB(0, 255, 0), 0, 2, 0); 
		}
	}
	else
	{
		cvDestroyWindow( "src" );
		cvDestroyWindow( "contour" );
		cvReleaseMemStorage(&storage);
		return -1;
	}


	cvShowImage( "contour", pContourImg );
	cvWaitKey(0);

	cvDestroyWindow( "src" );
	cvDestroyWindow( "contour" );

	cvReleaseImage( &pImg ); 
	cvReleaseImage( &pContourImg ); 
	cvReleaseMemStorage(&storage);
	return 0;
}

//功能测试区
//int main()
//{
//	IplImage *src = cvLoadImage("samples\\200.png");
//	IplImage *gray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
//	cvCvtColor(src,gray,CV_BGR2GRAY);
//	//cvCanny(gray, gray, 100, 255);
//	cvThreshold(gray, gray, 100, 255, CV_THRESH_BINARY_INV);
//	cvSmooth(gray,gray,2);
//	IplImage*bin = cvCloneImage(gray);
//	
//	CvMemStorage * storage = cvCreateMemStorage(0);
//	CvSeq * contour = 0;
//	//cvFindeContours（）会修改gray图像
//	int Num = cvFindContours(bin, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
//
//	for (; contour != 0; contour = contour->h_next){
//		cvDrawContours(src, contour, CV_RGB(0, 255, 0), CV_RGB(0, 255, 0), 0, 1, 0);
//	}
//	cvNamedWindow("Result", 1);
//	cvShowImage("Result", src);
//	//contour3(0, NULL);
//	cvWaitKey();
//
//	return 0;
//}