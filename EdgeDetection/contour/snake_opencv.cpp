// snake_opencv.cpp : 定义控制台应用程序的入口点。
//用opencv实现的用snake求物体边缘的例子。执行时先框住一个物体，
//然后就根据snake算法求最小能量曲线，即得到物体的边缘。框子越接近物体，效果越好。 运行需要opencv库
//Source:http://www.pudn.com/downloads104/sourcecode/graph/texture_mapping/detail428866.html

#include "stdafx.h"
#include <iostream>
#include <string.h>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <fstream>
#include <vector>
#include <opencv2/legacy/legacy.hpp>


using namespace std;



IplImage *image = NULL;
IplImage *image2 = NULL;

int Thresholdness = 141;
int ialpha = 20;
int ibeta=20; 
int igamma=20; 

CvPoint prev_pt;
CvPoint pt;

CvRect rect;

void process(int pos);

void DrawRectangle()
{
	cvRectangle(image2, prev_pt, pt, CV_RGB(255, 0 ,0), 1, 8, 0);
	
	cvShowImage("win1", image2);
}

void on_mouse( int event, int x, int y, int flags, void* chu)//opencv中的鼠标响应事件
{  
	if( !image2 )      
		return;    

	if( event == CV_EVENT_LBUTTONDOWN )     
	{
		prev_pt.x = x;
		prev_pt.y = y;
	}
	else if( event == CV_EVENT_LBUTTONUP )     
	{
		pt.x = x;
		pt.y = y;

		rect.x = prev_pt.x;
		rect.y = pt.y;
		rect.width = pt.x-prev_pt.x;
		rect.height = pt.y - prev_pt.y;

		DrawRectangle();

		process(0);

		//rect = cvRect(prev_pt.x, pt.y, pt.x-prev_pt.x, pt.y-prev_pt.y);
		
	}   
	//else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )   
	//{    
	//	CvPoint pt = cvPoint(x,y);     
	//	if( prev_pt.x < 0 )         
	//		prev_pt = pt;        
	//	/*cvLine( inpaint_mask, prev_pt, pt, cvScalarAll(255), 5, 8, 0 ); 
	//	cvLine( img, prev_pt, pt, cvScalarAll(255), 5, 8, 0 ); */  
	//	cvRectangle( image2, prev_pt, pt, cvScalar(0,255,0,0), 1, 8, 0 );
	//	prev_pt = pt;       
	//	cvNamedWindow("image");
	//	cvShowImage( "image", image2 );  
	//}
}

void process(int pos)
{
	/*if(image2) cvReleaseImage(&image2);
	if(image) cvReleaseImage(&image);*/

	
	/*image2 = cvCreateImage(cvGetSize(image), image->depth, 3);
	cvCvtColor(image, image2, CV_GRAY2RGB);*/

	//cvThreshold(image,image,Thresholdness,255,CV_THRESH_BINARY); //分割域值    

	//CvMemStorage* storage = cvCreateMemStorage(0);
	//CvSeq* contours = 0;

	//cvFindContours( image, storage, &contours, sizeof(CvContour), //寻找初始化轮廓
	//	CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE );

	//if(!contours) return ; 
	///*int length = contours->total;*/    
	//int length;
	//for (; contours!=NULL; contours=contours->h_next)
	//{
	//	length = contours->total;

	//	if (length > 10)
	//	{
	//		break;
	//	}
	//}

	////if(length<10) return ; 
	//CvPoint* point = new CvPoint[length]; //分配轮廓点

	//CvSeqReader reader;
	//CvPoint pt= cvPoint(0,0);;    
	//CvSeq *contour2=contours;    

	//cvStartReadSeq(contour2, &reader);
	//for (int i = 0; i < length; i++)
	//{
	//	CV_READ_SEQ_ELEM(pt, reader);
	//	point[i]=pt;
	//}

	//cvReleaseMemStorage(&storage);

	////显示轮廓曲线
	//for(int i=0;i<length;i++)
	//{
	//	int j = (i+1)%length;
	//	cvLine( image2, point[i],point[j],CV_RGB( 255, 0, 0 ),1,8,0 ); 
	//}


	vector <CvPoint> vec;

	int x = rect.x;//左上角点的x坐标
	int y = rect.y;//右下角点的y坐标
	int width = rect.width;//矩形宽
	int height = rect.height;//矩形高

	//CvRect rect = cvRect(x, y, width, height);

	//cvRectangle(image2, cvPoint(x, y-height), cvPoint(x+width, y), CV_RGB(0, 0, 0), 1, 8, 0);//画出矩形

	
	
	
	

	int num1 = width/5;
	int num2 = height/5;

	vec.push_back(cvPoint(x, y-height));

	for (int i=1; i<num1; i++)
	{
		vec.push_back(cvPoint(x+i*5, y-height));
	}

	vec.push_back(cvPoint(x+width, y-height));

	for (int i=1; i<num2; i++)
	{
		vec.push_back(cvPoint(x+width, y-height+i*5));
	}

	vec.push_back(cvPoint(x+width, y));

	for (int i=1; i<num1; i++)
	{
		vec.push_back(cvPoint(x+width-i*5, y));
	}

	vec.push_back(cvPoint(x, y));

	for (int i=1; i<num2; i++)
	{
		vec.push_back(cvPoint(x, y-i*5));
	}

	

	int n = vec.size();

	CvPoint *point = new CvPoint[n];

	for (int i=0; i<n; i++)
	{
		point[i] = vec[i];
	}

	/*for(int i=0;i<n;i++)
	{
		int j = (i+1)%n;
		cvLine( image2, point[i],point[j],CV_RGB( 255, 0, 0 ),1,8,0 ); 
	}*/

	float alpha=ialpha/100.0f; 
	float beta=ibeta/100.0f; 
	float gamma=igamma/100.0f; 

	CvSize size; 
	size.width=3; 
	size.height=3; 

	CvTermCriteria criteria; 
	criteria.type=CV_TERMCRIT_ITER; 
	criteria.max_iter=1000; 
	criteria.epsilon=0.1; 

	//注意cvSnakeImage函数中处理的必须是灰度图象
	cvSnakeImage( image, point, n, &alpha, &beta, &gamma, CV_VALUE, size, criteria, 0 ); 

	//显示曲线
	for(int i=0;i<n;i++)
	{
		int j = (i+1)%n;
		cvLine( image2, point[i],point[j],CV_RGB( 0, 255, 0 ), 1, 8, 0 ); 
	}

	cvNamedWindow("result", 1);
	cvShowImage("result", image2);

	delete []point;

}

int _tmain(int argc, _TCHAR* argv[])
{
	
	cvNamedWindow("win1", 1); 
	

	image2 = cvLoadImage("snake.jpg", 1); //显示图片
	image= cvLoadImage("snake.jpg", 0);

	/*cvCreateTrackbar("Thd", "win1", &Thresholdness, 255, process);
	cvCreateTrackbar("alpha", "win1", &ialpha, 100, process);
	cvCreateTrackbar("beta", "win1", &ibeta, 100, process);
	cvCreateTrackbar("gamma", "win1", &igamma, 100, process);*/
	//cvResizeWindow("win1",300,500);
	

	cvShowImage("win1",image2);
	

	cvSetMouseCallback( "win1", on_mouse, 0 );//触发鼠标事件执行process函数

	/*process(0);*/

	//cvNamedWindow("result", 1);
	//cvShowImage("result",image2);
	cvSaveImage("image.bmp", image2);

	cvWaitKey(0);

	cvReleaseImage(&image2);
	cvReleaseImage(&image);

	cvDestroyAllWindows();
	/*for(;;)
	{
		if(cvWaitKey(40)==27) break;
		cvShowImage("win1",image2);
	}*/


	return 0;
}

