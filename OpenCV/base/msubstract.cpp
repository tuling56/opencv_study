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

int msubstact1(string s1,string s2)
{
	Mat img1 = imread(s1);
	Mat img2 = imread(s2);//两幅图像的大小需要一致 
	Mat img_result1, img_result2, img_result;
	subtract(img1, img2, img_result1);
	//subtract(img2, img1, img_result2);
	//add(img_result1, img_result2, img_result);
	imwrite("s1-s2.jpg", img_result1);
	imshow("s1-s2", img_result1);
	waitKey(0);
	return 0;
}


int msubstact2(string s1,string s2)
{
	Mat img1 = imread(s1);
	if (!img1.data){
		printf("fail to load\n");
	}
	Mat img2 = imread(s2);
	Mat diff(img1.size(),CV_8UC1,0);
	Mat rev_img1;
	img1.convertTo(rev_img1,-1,-1,255);
	absdiff(rev_img1, img2, diff);
	imshow("-s1-s2", diff);
	imwrite("-s1-s2.png", diff);
	waitKey();
	return 0;
}

/*
 *	功能：视频多帧平均求背景
 *  输入：
 *  输出：
 *  状态：
 */

int mutiframeMean(char*videoname)
{
	Mat frame,tempframe;
	VideoCapture cap(videoname);
	if (!cap.isOpened()){
		cout << "open fail" << endl;
		return -1;
	}
	cap >> frame;
	Mat framesum;
	framesum.create(frame.rows, frame.cols,CV_32FC3);
	framesum.setTo(0);
	framesum.copyTo(tempframe);

	int curframe=0,start=0, meannum = 3;
	while (!frame.empty()) //处理多少帧
	{
		curframe=cap.get(CV_CAP_PROP_POS_FRAMES);
		start = (curframe - meannum > 1) ? (curframe - meannum) : 1;
		cap.set(CV_CAP_PROP_POS_FRAMES, start);

		while (cap.grab() && start <curframe)
		{
			cap.read(frame);// 获取当前帧
			convertScaleAbs(frame, tempframe, 1);
			accumulateWeighted(frame, framesum,0.02);

			//char testname[100];
			//sprintf(testname, "%s%d%s", "image", start, ".jpg");
			//imwrite(testname, frame);
			start++;
		}
		framesum.convertTo(framesum, -1, 1 / meannum);
		frame.convertTo(tempframe, 1, 1);
		absdiff(tempframe, framesum, tempframe);
		imshow("diff", tempframe);
		waitKey(10);
		cap >> frame;
	}
	return 0;
}


/*
 *	功能：图像添加噪声后和原来图像的差
 *  参数：
 *  状态：
 */
int msubstact3()
{
	Mat src = imread("samples//scene//111-1113_IMG.JPG");
	//为dst添加噪声
	RNG rnd = theRNG();
	for (int k = 0; k < 200; ++k)
	{
		int i = rand() % src.rows;
		int j = rand() % src.cols;
		src.at<Vec3b>(i, j)[0] = rnd(255);
		src.at<Vec3b>(i, j)[1] = rnd(255);
		src.at<Vec3b>(i, j)[2] = rnd(255);
	}

	//meanshift_smooth_segmentation();
	Mat dst;// = src.clone();
	Mat diff;

	//pyrMeanShiftFiltering(src, dst, 3, 2, 1);
	GaussianBlur(src, dst, Size(5, 5), 1.2, 1.2);
	absdiff(src, dst, diff);
	imshow("dst", dst);

	imshow("diff", diff);
	waitKey();

	return 0;
}



//功能测试区
//int main(int argc,char*argv[])
//{
//	//if (argc != 3){
//	//	printf("Please select the two pics\n");
//	//	waitKey(0);
//	//	return -1;
//	//}
//	//cout << "[method1]:s1-s2" << endl;
//	//msubstact1(argv[1], argv[2]);
//	//cout << "[method1]:-s1-s2" << endl;
//	//msubstact2(argv[1], argv[2]);
//	mutiframeMean("samples\\video\\test_o2.mp4");
//
//	waitKey(0);
//	return 0;
//}
