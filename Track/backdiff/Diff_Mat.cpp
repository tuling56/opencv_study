#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int diff_mat_main()
{
	VideoCapture cap("bike.avi");
	Mat frame,gray;
	for (;;)
	{
		cap>>frame;
		cvtColor(frame,gray,CV_BGR2GRAY);
		Mat bg(gray),fr,bin; //背景和前景
		absdiff(gray,bg,fr);//当前帧－背景得到前景
		accumulateWeighted(gray,bg,0.03);//更新背景
		threshold(fr,bin,50,255,THRESH_BINARY);//值化前景，进行统计，若变化大，则重新识别
		imshow("Bin",bin);
	}
	

}

