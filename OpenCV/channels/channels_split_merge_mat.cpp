/************************************************************************
* Copyright(c) 2014 tuling56
*
* File:	channels_split_merge_mat.cpp
* Brief: 各通道合图显示
* Source:http://wenku.baidu.com/link?url=u4sqa8GP8pw9Yj9js7ojH6xgzzAaILf6feE8fy2UuGnMX0rdWP-vL0Gj6ubZuuyji7VqhPCbpLT9-xP--800KNEueU0908ImXgxU0mfmE2O
* Status: 
* Date:	[11/30/2014 jmy]
************************************************************************/

#include <cv.h>
#include <highgui.h> 
#include <iostream>
#include <windows.h>  

using namespace cv; 
using namespace std;   

int channels_split_merge_mat()
{   
	/*
	FreeConsole();     //Hide the CMD window  
	if( argc != 2)   
	{    
		 std::cout <<"请输入需要加载的文件!" << std::endl;   
	     return -1;  
	}  
	*/
	//读取图像 
	Mat image;   
	//char * imageName = argv[1]; 
	char *imageName="samples/colorreduce.jpg";
	image = imread(imageName, CV_LOAD_IMAGE_COLOR);// Read the file  
	if(! image.data )   
	{ 
	    std::cout <<  "文件加载失败,请检查!" << std::endl;    
		return -1;   
    }   
	//灰度图  
	Mat grayImage;   
	cvtColor(image,grayImage,CV_RGB2GRAY);//Coonvert the image from RGB to Grayscale format  
	//显示用  
	Mat display;  
	int w = image.cols;  
	int h = image.rows;   
	display.create (h*2,w*2,CV_8UC3);  
	//黑色 
	Mat bk;  
	bk.create(h,w,CV_8UC1); 
	bk = Scalar(0);  
	//显示框   
	Mat m(display,Rect(0,0,w,h));  
	Mat m1(display,Rect(0,h,w,h));  
	Mat m2(display,Rect(w,0,w,h)); 
	Mat m3(display,Rect(w,h,w,h)); 
	Mat m4(display,Rect(w/2,h/2,w,h)); 
	//通道分离用  
	std::vector<cv::Mat> sbgr(image.channels());  
	//通道合成用   
	std::vector<cv::Mat> mbgr(image.channels()); 
	//分离   
	split(image,sbgr); //split to sbgr[0] ,sbgr[1] ,sbgr[2]  
	//合成到显示-灰度 
	mbgr[0] = grayImage; 
	mbgr[1] = grayImage; 
	mbgr[2] = grayImage; 
	merge(mbgr,m);  
	//合成到显示-B  
	mbgr[0] = sbgr[0]; 
	mbgr[1] = bk;  
	mbgr[2] = bk;  
	merge(mbgr,m1);  
	//合成到显示-G 
	mbgr[0] = bk; 
	mbgr[1] = sbgr[1]; 
	mbgr[2] = bk;  
	merge(mbgr,m2); 
	//合成到显示-R   
	mbgr[0] = bk; 
	mbgr[1] = bk; 
	mbgr[2] = sbgr[2]; 
	merge(mbgr,m3);  
	//复制到显示-原图  
	image.copyTo (m4); 
	//显示   
	namedWindow( imageName,CV_WINDOW_NORMAL);// Create a window for display. 
	resizeWindow(imageName,w*2,h*2);  
	imshow(imageName, display);    // Show our image inside it.  
	waitKey(0);           // Wait for a keystroke in the window   
	return 0; 
} 