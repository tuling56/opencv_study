/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	getRGB.cpp
* Brief: 提取出RGB通道，并在一个窗口上显示所有通道，然后做成视频
* Source:http://blog.csdn.net/merlin_q/article/details/7078652
* Status: 
* Date:	[1/10/2015 jmy]
************************************************************************/


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat process(Mat &img)
{
	Mat src;
	pyrDown(img, src);
	Mat bgr_res[3];
	Mat bgr[3];
	for( int j = 0 ; j < 3; j++)
	{
		split(src, bgr);
		for (int i=0; i<3; ++i)
		{
			if (i != j )
				bgr[i] = Mat::zeros(src.size(), bgr[0].type());
		}
		merge(bgr, 3, bgr_res[j]);
	}
	Mat dst(img.size() + Size(10, 10), src.type());
	Mat ROI(dst, Rect(0,0,src.cols, src.rows));
	src.copyTo(ROI);
	Mat ROI_b(dst, Rect(dst.cols-src.cols, 0, src.cols, src.rows));
	bgr_res[0].copyTo(ROI_b);
	Mat ROI_g(dst, Rect(0, dst.rows-src.rows, src.cols, src.rows));
	bgr_res[1].copyTo(ROI_g);
	Mat ROI_r(dst, Rect(dst.cols-src.cols, dst.rows-src.rows, src.cols, src.rows));
	bgr_res[2].copyTo(ROI_r);
	return dst;
}

//功能测试区
//int main()
//{
//	VideoCapture cap("samples/video/test_o2.mp4");
//	if (!cap.isOpened()){
//		cout << "could not open" << endl;
//		return -1;
//	}
//
//	//视频读写
//	Mat frame;
//	cap >> frame;
//	Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),(int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));
//	VideoWriter w;
//	w.open("samples/video/rgbchannel.avi", CV_FOURCC('X', 'V', 'I', 'D'), cap.get(CV_CAP_PROP_FPS), S + Size(10,10)	);
//
//	Mat dst;
//	do
//	{
//		cap >> frame;
//		dst = process(frame);
//		w << dst;
//	}while (frame.data);
//
//	cout << "done" << endl;
//	return 0;
//}