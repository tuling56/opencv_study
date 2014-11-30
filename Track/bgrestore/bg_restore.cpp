//Function：高斯混合背景建模和前景检测
//Source:http://blog.csdn.net/xiaowei_cqu/article/details/23689189
//Status:
//Improve:
//Info:[11/18/2014 jmy]



#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int bg_restore_main()
{
	VideoCapture video("bike.avi");
	Mat frame,mask,thresholdImage, output;
	int frameNum=0;
	video>>frame;
	BackgroundSubtractorMOG2 bgSubtractor(20,16,true); //背景建模

	while(true)
	{
		video>>frame;
		if (frame.empty())
		{
			cout<<"视频结束\n"<<endl;
			return -1;
		}
		++frameNum;
		bgSubtractor(frame,mask,0.001); //mask为前景
		bgSubtractor.getBackgroundImage(output);
		cout<<frameNum<<endl;
		imshow("前景",mask);
		imshow("背景",output);
		waitKey(10);
	}
	return 0;
}