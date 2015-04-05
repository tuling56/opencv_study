/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	main.cpp
* Brief: lsd直线检测的原理及具体程序实现，接口是lsd.c及lsd.h
* Source:http://www.pudn.com/downloads538/sourcecode/graph/texture_mapping/detail2223326.html
* Status: 可运行
* Date:	[3/13/2015 jmy]
************************************************************************/

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cmath>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>

extern "C"
{
#include "lsd.h"
};

using namespace std;

int lsdLineDete(int argc, char *argv[])
{
	ntuple_list detected_lines;

	//载入一幅图像，路径需要改变
	string path_image = "samples//test.jpg";
    IplImage * m_im;
	m_im=cvLoadImage(path_image.c_str(),CV_LOAD_IMAGE_COLOR);
	
	clock_t start,finish;
	double duration,rate;
	start = clock();
	//lsd函数处理灰度图像，需要进行转换
	IplImage* im_gray = cvCreateImage(cvSize(m_im->width,m_im->height),IPL_DEPTH_8U,1);
	cvCvtColor(m_im,im_gray,CV_RGB2GRAY);
	image_double image = new_image_double(im_gray->width, im_gray->height);
	BYTE* im_src = (BYTE*)im_gray->imageData;
	int xsize = image->xsize;//宽度
	int ysize = image->ysize;//高度
	int y,x;

	for (y = 0;y < ysize;y++)
	{
		for (x = 0;x < xsize;x++)  //x是横坐标，y是纵坐标
		{
			image->data[y*xsize + x] = im_src[y*im_gray->widthStep + x];//im_gray是灰度图像，没有颜色通道
		}
	}
	cvReleaseImage(&im_gray);
	detected_lines = lsd(image);//detected_lines中存储提取直线的首位坐标及宽度，具体意义见说明文档
	free_image_double(image);
	finish = clock();
	duration = (double)(finish - start)/CLOCKS_PER_SEC;
	cout<<"total time of extract lines is:"<<duration<<endl;//提取所有直线需要的总时间
	rate = duration/detected_lines->size;
	cout<<"time of extract per line is :"<<rate<<endl;//提取一条直线需要的平均时间
	
	//将检测出的直线绘制在载入的彩色图像上
	int dim = detected_lines->dim;
	IplImage* res_im = (IplImage*)cvClone(m_im); //要在彩色图上画直线，对原图做拷贝

	vector<double> angle(detected_lines->size,0);
	double angletemp;
	
	for (unsigned int j = 0;j < detected_lines->size;j++)
	{
		CvPoint start_pt = cvPoint((int)detected_lines->values[j*dim+0],(int)detected_lines->values[j*dim+1]);
		CvPoint end_pt = cvPoint((int)detected_lines->values[j*dim+2],(int)detected_lines->values[j*dim+3]);
		cvLine(res_im,start_pt,end_pt,CV_RGB(j%255,(5*j)%255,(9*j)%255),1,CV_AA);
		
		angletemp = (int)(atan((detected_lines->values[j*dim+1]-detected_lines->values[j*dim+3])/(detected_lines->values[j*dim+0]-detected_lines->values[j*dim+2]))*180/3.1416);
		angle[j] = angletemp;
		cout<<"angle "<<j<<" = "<<angle[j]<<endl;
	}

	vector<int> num(detected_lines->size,0);
	for(unsigned int i = 0;i<= detected_lines->size-1;i++)
	{
		num[i] = i;
	}
	double temp;
	int numtemp;
	for(unsigned int i = 0;i < detected_lines->size-1;i++)         //从小到大排列并返回线段序列号，num[]存储线段序列号
	{
		for(unsigned int j = i+1;j<= detected_lines->size-1;j++)
		{ 
			if(angle[i] > angle[j])
			{ 
				temp = angle[i]; numtemp = num[i];
				angle[i] = angle[j]; num[i] = num[j];
				angle[j] = temp;num[j] = numtemp;
			}
		}
	}

	for(unsigned int j = 0;j<= detected_lines->size-1;j++)
	{ 
		cout<<"角度为 "<<angle[j]<<" 时，直线序号为 "<<num[j]<<endl;
	}
	
	vector<double> angle_m(detected_lines->size);    //存储相同倾角的线段
	vector<int> angle_sumMAX;     //存储相同倾角线段分类后每组线段数目最大值
	vector<int> angle_num_N;
	vector<vector<int>> angle_num;   //存储线相同倾角分类后每一类的线段序列号，其中第一个表示类号，第二个表示这一类中第几条线段
	
	int m = 0;      //设置初始值
	angle_m.push_back(angle[0]);     //设置相同倾角的线段的第一个值为angle[0]
	angle_num_N.push_back(num[0]);      //设置线段初始序列号
	int a = 1;      //设置相同倾角的线段的第一类线段数目初始值为1
	for(unsigned int j = 1;j < detected_lines->size;j++)     //分类
	{
		if (angle[j] == angle[j-1])
		{
			a++;
			angle_num_N.push_back(num[j]);   //每一类每一条线段的序列号
			if(j == detected_lines->size-1)
			{
				angle_sumMAX.push_back(a);
				angle_num.push_back(angle_num_N);
			}
		}
		else
		{
			angle_sumMAX.push_back(a);
			angle_num.push_back(angle_num_N);
			a=1;      //设置相同倾角的线段的下一类线段数目初始值为1
			m++;
			angle_m[m] = angle[j];
			angle_num_N[0] = num[j];    //这一类第一条线段的序列号
			if(j == detected_lines->size-1)
			{
				angle_num_N.push_back(num[j]);
				angle_num.push_back(angle_num_N);
			}

		}
		/*cout<<"m = "<<m<<endl;*/
	}
	
	const int mMAX = m+1;   //总类数
	//cout<<"mMAX = "<<mMAX<<endl;

	//for( m = 0;m <= mMAX-1;m++)
	//	cout<<"angle_sumMAX "<<m<<" = "<<angle_sumMAX[m]<<endl;

	//for(unsigned int i = 0;i < mMAX;i++)         //从小到大排列并返回线段序列号，num[]存储线段序列号
	//{
	//	for(unsigned int j = 0;j< angle_sumMAX[i];j++)
	//	{ 
	//		cout<<"第 "<<i<<" 类"<<"第 "<<j<<" 条直线   "<<angle_num[i][j]<<endl;
	//	}
	//}
	
	vector<CvPoint> pt_N;
	pt_N.reserve(100);
	vector<vector<CvPoint>> pt;   //存储线相同倾角分类后每一类的不同线段的首尾点坐标，其中第一个表示起点，第二个表示末点
	vector<CvPoint> pt_m(mMAX);   //交点的平均坐标
	
	for (m = 0;m <= mMAX-1;m++)
	{
		int n = 0;
		int b = 0;
		double y0,y1,x0,x1,y2,y3,x2,x3,k1,k2,x,y;
		if(angle_sumMAX[m]>=3)
		{
			cout<<"m = "<<m<<"时，共有"<<angle_sumMAX[m]<<"条直线"<<endl;
			for (int i = 0;i < angle_sumMAX[m]-1;i++)
			{
				for (int j = i+1;j <= angle_sumMAX[m]-1;j++ )
				{
					/*detected_lines->values[j*dim+0]和detected_lines->values[j*dim+1]分别是线段一个端点的x、y坐标，detected_lines->values[j*dim+2]和detected_lines->values[j*dim+3]分别是线段另一个端点的x、y坐标，detected_lines->values[j*dim+4]为线段的宽度。其中，j为某一条线段的索引号。*/
					y0 = detected_lines->values[angle_num[m][i]*dim+1];
					y1 = detected_lines->values[angle_num[m][i]*dim+3];
					x0 = detected_lines->values[angle_num[m][i]*dim+0];
					x1 = detected_lines->values[angle_num[m][i]*dim+2];
					y2 = detected_lines->values[angle_num[m][j]*dim+1];
					y3 = detected_lines->values[angle_num[m][j]*dim+3];
					x2 = detected_lines->values[angle_num[m][j]*dim+0];
					x3 = detected_lines->values[angle_num[m][j]*dim+2];
					k1 = (y0-y1)/(x0-x1);
					k2 = (y2-y3)/(x2-x3);
					x = (k1*x0-k2*x2+y2-y0)/(k1-k2);
					y = y0+(x-x0)*k1;
					/*cout<<"x = "<<x<<endl;
					cout<<"y = "<<y<<endl;*/
					
					/*CvPoint pt_o;
					pt_o.x = 0;
					pt_o.y = 0;
					pt_N.push_back(pt_o);*/
					/*pt_N.push_back(cvPoint(0,0));
					pt_N[n].x = (int)x;
					pt_N[n].y = (int)y;*/
					
					pt_N.push_back(cvPoint(x, y));
					/*pt_N.insert(pt_N.end(), 1, point_now);*/
	
    //				pt_N[0] = point_now;
	//				pt.push_back(pt_N);		
	//				pt_N.push_back(pt_N[0]);			
					
					pt.push_back(pt_N);
					pt_m[m].x += pt_N[n].x;
					pt_m[m].y += pt_N[n].y;					
					n++;
				}
			}
			pt_m[m].x = pt_m[m].x/(angle_sumMAX[m]*(angle_sumMAX[m]-1)/2);
			pt_m[m].y = pt_m[m].y/(angle_sumMAX[m]*(angle_sumMAX[m]-1)/2);
		}
		if((pt_m[m].x >= 0)&&(pt_m[m].y >= 0))
		cvCircle(res_im,cvPoint((int)pt_m[m].x,(int)pt_m[m].y),100,CV_RGB(255,0,0),-1,CV_AA,4);
	}

	cvShowImage("Image",res_im);
	cvWaitKey();

	return 0;
}