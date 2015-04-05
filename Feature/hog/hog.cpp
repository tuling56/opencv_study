/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	hog.cpp
* Brief: 
* Source:http://blog.csdn.net/augusdi/article/details/9005352
* Status: 缺失未调通
* Date:	[3/26/2015 jmy]
************************************************************************/

#include <opencv2/core/core.hpp> 
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>  //HogDescriptor描述子所在的头文件
#include <iostream>

using namespace cv;


int hog_feature(string filename)
{
	Mat trainImg; 
	trainImg=imread(filename,0);  

	HOGDescriptor *hog=new HOGDescriptor(cvSize(3,3),cvSize(3,3),cvSize(5,10),cvSize(3,3),9);  //具体意思见参考文章1,2
	vector<float>descriptors;//结果数组
	hog->compute(trainImg, descriptors,Size(1,1), Size(0,0)); //调用计算函数开始计算
	printf("%d\n",descriptors.size()); //打屏一下结果数组的大小，看看是否符合文献2的预估， 发现完全一样，那篇文章很给力

	/*
	sift("1.jpg");  //这是我改写的sift...
	for (int i=0;i<kp.size();i++) 
	{  
		//这个循环就是我用来提前特征点附近3*3区域的梯度信息统计
		printf("keypoint %d at %f %f\n",i,kp[i].first,kp[i].second);
		if (kp[i].first==picw) 
			kp[i].first--;
		if (kp[i].first==0) 
			kp[i].first=1;
		if (kp[i].second==pich) 
			kp[i].second--;
		if (kp[i].second==0) 
			kp[i].second=1;
		int pos=(kp[i].second-1)*(picw-2)+kp[i].first-1;
		for (int j=0;j<9;j++) 
		{
			res[j]+=descriptors[pos*9+j];
		}
	}
	puts("result:");
	for (int i=0;i<9;i++)
		printf("%lf ",res[i]); //结果以文字输出。。。
	puts("");
	
	IplImage * respic;  //结果以直方图输出，里面有个res.jpg是我画的直方图背景图，没有这个图跑不了程序，把从这以下代码注释掉吧
	if ((respic = cvLoadImage("res.jpg", 1)) == 0) 
		return -1; 
	double maxx=0;
	for (int i=0;i<9;i++) if (maxx<res[i]) maxx=res[i];
	for (int i=0;i<9;i++)
		cvRectangle(respic, cvPoint(150+51*i,(maxx-res[i])/maxx*(352-77)+77),
		cvPoint(201+51*i,351), CV_RGB(0, 0, 0),
		1, 8, 0);
	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0 );
	itoa((int)maxx,outs,10);
	cvPutText(respic,outs,cvPoint(53,83),&font,CV_RGB(0, 0, 0));
	cvNamedWindow("image1",1);
	cvShowImage("image1", respic);
	cvWaitKey(0);  
	cvDestroyWindow("image1");
	*/

	return 0;
}


//功能测试区
int main(int argc,char**argv)
{
	hog_feature("samples//zimu.jpg");

    std::cin.get();
	return 0;
}
