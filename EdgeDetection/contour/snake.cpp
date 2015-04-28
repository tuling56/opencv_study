/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	Snake_plus.cpp
* Brief: 用Snake算法实现轮廓提取
* Source:博文：http://blog.csdn.net/crond123/article/details/3958807
         下载：http://www.pudn.com/downloads327/sourcecode/graph/texture_mapping/detail1440517.html
* Status: 可运行
* Date:	[4/6/2015 jmy]
************************************************************************/
#include "cv.h"
#include "highgui.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <cassert>
#include <windows.h>
#include <legacy/legacy.hpp>

std::vector<CvPoint>   InitContour; 

IplImage* temp;  //= cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
void showContent(IplImage * img)
{
	if(temp==NULL)
		temp= cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cvCopyImage(img,temp);
	for (int i=0;i<InitContour.size();i++)
	{
		cvCircle(temp, InitContour[i], 2, cvScalarAll(155));	
		int j = (i+1)%InitContour.size();
		cvLine( temp,  InitContour[i],InitContour[j] , cvScalarAll(100), 1);           
	}
	cvSaveImage("sef.jpg",temp);
	cvShowImage( "srcImage", temp );

}


double wcvDistance(CvPoint &pt1,CvPoint &pt2)
{
	double distance = sqrt(pow((double)(pt1.x - pt2.x),2)+pow((double)(pt1.y - pt2.y),2));
	return distance;
}

void CaculateDiff(std::vector<CvPoint> &src,std::vector<double> &diff)
{
	int size = src.size();
	diff.clear();
	diff.reserve(size);
	diff.resize(size,0.0);

	for(int i=0;i<size;i++)
	{
		int j = (i+1)%size;
		diff[i] = wcvDistance(src[i],src[j]);
	}
}

void removeNearPoints(std::vector<CvPoint> &src,std::vector<CvPoint> &dst,double max_length)
{
	std::vector<CvPoint> result;
	result.clear();

	std::vector<CvPoint> temp;
	temp.clear();
	temp = src;

	std::vector<double> diff ;	
	int removed = 1;
	while(removed!=0)
	{
		//diff.clear();
		result.clear();
		removed = 0;
		CaculateDiff(temp,diff);
		int srcsize = temp.size();
		for(int i=0;i<srcsize;i++)
		{
			if(diff[i]<max_length&&(i%2==0))
			{
				removed++;
				continue;
			}
			result.push_back(temp[i]);
		}
		temp.clear();
		temp = result;
	}

	dst.clear();
	dst  = result;
	return ;
}


void contourInterpolation(std::vector<CvPoint> &src,std::vector<CvPoint> &dst)
{
	std::vector<CvPoint> temp;
	std::vector<double> diff ;
	CaculateDiff(src,diff);
}

void on_mouse( int event, int x, int y, int flags, void* ptr)
{
	if( event == CV_EVENT_LBUTTONDOWN )
	{
		InitContour.push_back(cvPoint(x,y));
		showContent((IplImage *)ptr);
	}
}

//对所有轮廓点进行插值
void ReGeneralPtArrs(std::vector<CvPoint> &src,std::vector<CvPoint> &dst)
{
	std::vector<CvPoint> temp  = src;
	std::vector<double> diff ;
	for(int k=0;k<10;k++)
	{
		int size = temp.size();
		CaculateDiff(temp,diff);
		std::vector<CvPoint> na;
		for (int i=0;i<temp.size();i++)
		{
			if(diff[i]<0.5)
				continue;
			na.push_back(temp[i]);
			if(diff[i]>=1.8)
			{
				int next = (i+1)%size;
				CvPoint ne = temp[next];
				CvPoint mid = cvPoint((temp[i].x+ne.x)/2,(temp[i].y+ne.y)/2);
				na.push_back(mid);
			}
		}
		temp.clear();
		temp = na;
	}

	dst = temp;

}
int main(int argc,char* argv[])
{
	IplImage * srcimage = NULL;
	if ( argc == 2)
		srcimage = cvLoadImage((char *)argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	else
		srcimage = cvLoadImage("snake_test.jpg", CV_LOAD_IMAGE_GRAYSCALE);  
	
	if(srcimage==NULL){
		std::cout<<"error file name"<<std::endl;
		return -1;
	}

	InitContour.clear();
	cvNamedWindow("srcImage");	
	cvShowImage("srcImage", srcimage);
	cvSetMouseCallback("srcImage", on_mouse,srcimage);

	cvWaitKey(); //等待迭代开始

	float alpha=0.01;
	float beta=0.1;
	float gamma=0.1;

	CvSize size;
	size.width=3;
	size.height=3;
	
	CvTermCriteria criteria;
	criteria.type=CV_TERMCRIT_ITER;
	criteria.max_iter=5;
	criteria.epsilon=0.1;
	int itetime =500 ;
	for(int ite=0;ite<itetime;ite++)
	{
		//对边界点查值，使所有边界点距离为(1-2)
		ReGeneralPtArrs(InitContour,InitContour);
		//将边界点的平均距离改为3;根据我的经验，距离大一点效果要好一些
		removeNearPoints(InitContour,InitContour,3);
		CvPoint*  pts = new  CvPoint[InitContour.size()];
		for (int i=0;i<InitContour.size();i++)
		{
			pts[i] = InitContour[i];
		}
		if(InitContour.size()<5){
			printf("迭代失败！\n");
			break;
		}

		cvSnakeImage( srcimage, pts,InitContour.size(),&alpha,&beta,&gamma,CV_VALUE,size,criteria,1);

		int size = InitContour.size();
		InitContour.clear();
		for (int i=0;i<size;i++)
		{
			if(pts[i].x<0||pts[i].x>=srcimage->width)
				continue;
			if(pts[i].y<0||pts[i].y>=srcimage->height)
				continue;

			InitContour.push_back(pts[i]);

		}

		showContent(srcimage);
		cvWaitKey(20);
		Sleep(200);
		printf("迭代了 %d 次\n",ite);
		delete []pts;	

		char c = cvWaitKey(1);
		if ((c&255)==27)
			break;
	}

    removeNearPoints(InitContour,InitContour,6);
	showContent(srcimage);
	cvWaitKey();

	cvReleaseImage(&srcimage);
	cvReleaseImage(&temp);

	return 0;
}


