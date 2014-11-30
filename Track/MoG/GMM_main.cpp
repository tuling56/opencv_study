// This is based on the "An Improved Adaptive Background Mixture Model for
// Real-time Tracking with Shadow Detection" by P. KaewTraKulPong and R. Bowden
// Author : zouxy
// Date   : 2013-4-13
// HomePage : http://blog.csdn.net/zouxy09
// Source: http://blog.csdn.net/zouxy09/article/details/9622401
// Email  : zouxy09@qq.com

#include "opencv2/opencv.hpp"
#include "MOG_BGS.h"
#include <iostream>
#include <cstdio>

using namespace cv;
using namespace std;

int GMM_main(int argc, char* argv[])
{
	Mat frame, gray, mask;
	VideoCapture capture;
	//capture.open("video.avi");
	capture.open(0);


	if (!capture.isOpened())
	{
		cout<<"No camera or video input!\n"<<endl;
		return -1;
	}

	MOG_BGS Mog_Bgs;
	int count = 0;

	while (1)
	{
		count++;
		capture >> frame;
		if (frame.empty())
			break;
		cvtColor(frame, gray, CV_RGB2GRAY);

		if (count == 1)
		{
			Mog_Bgs.init(gray);
			Mog_Bgs.processFirstFrame(gray);
			cout<<" Using "<<TRAIN_FRAMES<<" frames to training GMM..."<<endl;
		}
		else if (count < TRAIN_FRAMES)
		{
			Mog_Bgs.trainGMM(gray);
		}
		else if (count == TRAIN_FRAMES)
		{
			Mog_Bgs.getFitNum(gray);
			cout<<" Training GMM complete!"<<endl;
		}
		else
		{
			Mog_Bgs.testGMM(gray);
			mask = Mog_Bgs.getMask();
			morphologyEx(mask, mask, MORPH_OPEN, Mat());
			erode(mask, mask, Mat(7, 7, CV_8UC1), Point(-1, -1));  // You can use Mat(5, 5, CV_8UC1) here for less distortion
			dilate(mask, mask, Mat(7, 7, CV_8UC1), Point(-1, -1));
			imshow("mask", mask);
		}

		imshow("input", frame);	

		if ( cvWaitKey(10) == 'q' )
			break;
	}

	return 0;
}