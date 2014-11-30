//Function： OpenCV_基于自适应背景更新的运动目标检测
//Source:http://blog.csdn.net/icvpr/article/details/8487271
//Status: 运动检测的Mat类型版本
//Improve:
//Info:[11/25/2014 jmy]

#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	string videoFileName = "bike.avi";

	int threshold = 10;    // 二值化阈值
	float alpha = 0.003;     // 更新速度 [0, 1]

	VideoCapture capture;
	capture.open(videoFileName);
	if (!capture.isOpened())
	{
		cout<<"cannot open video"<<endl;
		return -1;
	}

	Mat foregroundImg;
	Mat foregroundMat;

	Mat backgroundImg;
	Mat backgroundMat;

	Mat frame;
	Mat grayImg;
	Mat grayMat;

	while (capture.read(frame))
	{
		cvtColor(frame, grayImg, CV_BGR2GRAY);
		grayImg.convertTo(grayMat, CV_32FC1);

		//对第一帧的处理
		if (backgroundMat.empty())
		{
			grayImg.copyTo(backgroundImg);
			grayImg.convertTo(backgroundMat, CV_32FC1);		
		}

		// 背景减除
		absdiff(grayMat, backgroundMat, foregroundMat);
		
		// 自适应背景更新
		addWeighted(backgroundMat, alpha, foregroundMat, 1-alpha, 0, backgroundMat);

		// 二值化，获取前景像素点
		cv::threshold(foregroundMat, foregroundMat, threshold, 255, CV_THRESH_BINARY);


		// 为了显示用，将CV_32FC1转换为CV_8U
		convertScaleAbs(foregroundMat, foregroundImg);
		convertScaleAbs(backgroundMat, backgroundImg);

		imshow("frame", frame);
		imshow("foreground", foregroundImg);
		imshow("background", backgroundImg);

		if (waitKey(2) > 0)
		{
			break;
		}
	}

	return 0;
}