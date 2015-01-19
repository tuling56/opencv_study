/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	msubstract.cpp
* Brief: 两幅图像比较
* Source:http://blog.csdn.net/merlin_q/article/details/7026375
* Status: 
* Date:	[1/9/2015 jmy]
************************************************************************/


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

string filename1 = "samples/(6)SWT_Transform.png";
string filename2 = "samples/(7)Connected Components.png";

int msubstact1()
{
	Mat img1 = imread(filename1);
	Mat img2 = imread(filename2);//两幅图像的大小需要一致 
	Mat img_result1, img_result2, img_result;
	subtract(img1, img2, img_result1);
	subtract(img2, img1, img_result2);
	add(img_result1, img_result2, img_result1);
	imwrite("result.jpg", img_result1);
	imshow("result", img_result1);
	waitKey(0);
	return 0;
}


int msubstact2()
{
	Mat img1 = imread(filename1);
	if (!img1.data){
		printf("fail to load\n");
	}
	Mat img2 = imread(filename2);
	Mat diff(img1.size(),CV_8UC1,0);
	Mat rev_img1;
	img1.convertTo(rev_img1,-1,-1,255);
	absdiff(rev_img1, img2, diff);
	imshow("diff", diff);
	waitKey();
	return 0;
}

/*
 *	功能：视频多帧平均求背景
 *  输入：
 *  输出：
 *  状态：
 */

int mutiMean(char*videoname)
{
	Mat frame;
	Mat framesum;
	VideoCapture cap(videoname);
	if (!cap.isOpened())
	{
		cout << "open fail" << endl;
		return -1;
	}
	cap >> frame;
	int meannum = 3;
	while (!frame.empty()) //处理多少帧
	{
		accumulate(frame, framesum);
	}
	return 0;
}





//功能测试区
//int main()
//{
//	msubstact1();
//	msubstact2();
//	waitKey(0);
//	return 0;
//}
