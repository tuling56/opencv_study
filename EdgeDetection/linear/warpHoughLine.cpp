/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	warpHoughLine.cpp
* Brief: opencv2实现多张图片路线路牌(直线和圆）检测并将处理后的图片合成视频
* Source:http://www.tuicool.com/articles/A36bie
* Status:垃圾股，不能太垃圾了
* Date:	[3/11/2015 jmy]
************************************************************************/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>  
#include <sstream>
#include "warpHoughLine.h"
#include <opencv2\opencv.hpp> 
using namespace cv;
using namespace std;

//#define PI 3.1415926

int warpHoughLine()
{
	stringstream ss;
	string str;
	stringstream sss;
	string strs;
	for (int i = 1; i <= 80; i++)
	{
		str = "E:\\360YPan\\OpenCV\\OpenCV_Github\\EdgeDetection\\samples\\scene\\";//选择F:\\图片\\中的5张图片  
		ss.clear();
		ss << str;
		ss << i;
		ss << ".JPG";
		ss >> str;
		Mat image = imread(str, 1);
		if (!image.data)   return 0;
		
		Mat img = image(Rect(0.4*image.cols, 0.58*image.rows, 0.4*image.cols, 0.3*image.rows));
		Mat contours;
		Canny(img, contours, 80, 100);

		Mat contoursInv;
		threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);

		// Display the image of contours
		/*namedWindow("Canny Contours");
		imshow("Canny Contours",contoursInv);*/

		// Create LineFinder instance
		LineFinder ld;

		// Set probabilistic Hough parameters
		ld.setLineLengthAndGap(60, 40);
		ld.setMinVote(30);

		vector<Vec4i> li = ld.findLines(contours);
		ld.drawDetectedLines(img);

		/*namedWindow(" HoughP");
		imshow(" HoughP",img);*/
		/*namedWindow("Detected Lines with HoughP");
		imshow("Detected Lines with HoughP",image);*/


		//-----------------------------------【圆检测】-----------------------------
		Mat imgGry;
		cvtColor(image, imgGry, CV_BGR2GRAY);
		GaussianBlur(imgGry, imgGry, Size(5, 5), 1.5);
		vector<Vec3f> circles;
		HoughCircles(imgGry, circles, CV_HOUGH_GRADIENT,
					2,   // accumulator resolution (size of the image / 2) 
					150,  // minimum distance between two circles
					200, // Canny high threshold 
					100, // minimum number of votes 
					25, 50); // min and max radius

		cout << "Circles: " << circles.size() << endl;

		// Draw the circles
		vector<Vec3f>::const_iterator itc = circles.begin();
		while (itc != circles.end()) 
		{
			circle(image,
				   Point((*itc)[0], (*itc)[1]), // circle centre
				   (*itc)[2], // circle radius
				   Scalar(255), // color 
				   2); // thickness
			++itc;
		}

		/*namedWindow(str);
		imshow(str,image);*/
		strs = "samples\\lineDete\\";//选择F:\\图片\\中的5张图片  
		sss.clear();
		sss << strs;
		sss << i;
		sss << ".jpg";
		sss >> strs;
		imwrite(strs, image);

	}


	//-----------------------------------【视频写部分】-----------------------------
	int num = 1;
	CvSize size = cvSize(1024, 960);  //视频帧格式的大小  
	double fps = 3;                   //每秒钟的帧率  
	CvVideoWriter *writer = cvCreateVideoWriter("1.avi", -1, fps, size); //创建视频文件  
	char cname[100];
	sprintf(cname, "samples\\lineDete\\%d.jpg", num); //加载图片的文件夹，图片的名称编号是1开始1，2,3,4,5.。。。  
	IplImage *src = cvLoadImage(cname);
	if (!src){
		return 0;
	}

	IplImage *src_resize = cvCreateImage(size, 8, 3); //创建视频文件格式大小的图片  
	cvNamedWindow("avi");
	while (src)
	{
		cvShowImage("avi", src_resize);
		cvWaitKey(1);
		cvResize(src, src_resize);		     //将读取的图片设置为视频格式大小相同  
		cvWriteFrame(writer, src_resize);	 //保存图片为视频流格式  
		cvReleaseImage(&src);				  //释放空间  
		num++;
		sprintf(cname, "samples\\lineDete\\%d.jpg", num);
		src = cvLoadImage(cname);       //循环读取数据  
	}
	cvReleaseVideoWriter(&writer);
	cvReleaseImage(&src_resize);


	waitKey();
	return 0;
}