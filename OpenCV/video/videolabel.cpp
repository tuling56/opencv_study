//Tools for Labeling Video
//Coded by L. Wei
//Date: 9/4/2013
/*
 *功能：视频标注
 *来自：http://blog.csdn.net/xiaowei_cqu/article/details/8778976
 *Satus:程序有一些问题
 *Info:[11/12/2014 jmy]
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using namespace cv;

// Global variables
bool is_drawing=false;
vector<Rect> biaozhu_boxs; //所有的标注矩形框
Rect drawing_box;
Mat img_original,img_drawing;

//说明
static void help()
{
	cout << "This program designed for labeling video \n"
		<<"Coded by L. Wei on 9/4/2013\n"<<endl;
	
	cout<<"Use the mouse to draw rectangle on the image for labeling.\n"<<endl;

	cout << "Hot keys: \n"
		"\tESC - quit the program\n"
		"\tn - next frame of the video\n"
		"\tz - undo the last label\n"
		"\tc - clear all the labels\n"
		<<endl;
}

//回调函数，显示矩形框
static void onMouse( int event, int x, int y, int, void* )
{
	switch(event)
	{
		case CV_EVENT_LBUTTONDOWN: 
			//the left up point of the rect
			is_drawing=true;
			drawing_box.x=x;
			drawing_box.y=y;
			break;
		case CV_EVENT_MOUSEMOVE:
			//adjust the rect (use color blue for moving)
			if(is_drawing)
			{
				drawing_box.width=x-drawing_box.x;
				drawing_box.height=y-drawing_box.y;
				img_original.copyTo(img_drawing);
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
				}
				rectangle(img_drawing,drawing_box,Scalar(255,0,0));
			}
			break;
		case CV_EVENT_LBUTTONUP:
			//finish drawing the rect (use color green for finish)
			if(is_drawing)
			{
				drawing_box.width=x-drawing_box.x;
				drawing_box.height=y-drawing_box.y;
				img_original.copyTo(img_drawing);
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
				}
				rectangle(img_drawing,drawing_box,Scalar(0,255,0));
				biaozhu_boxs.push_back(drawing_box);
			}
			is_drawing=false;
			break;
	}
	imshow("Video",img_drawing);
	return;
}


int videolabel_main()
{
	help();
	ofstream outfile("video_label.txt");

	//原始视频显示
	namedWindow("Video");
	VideoCapture capture("PicVideo/video/test_o2.mp4");
	capture>>img_original; //读取初始帧
	imshow("Video",img_original);
	
	setMouseCallback( "Video", onMouse, 0 ); //设置回调函数

	//绘制所有矩形,对第一帧的处理
	img_original.copyTo(img_drawing); //如果后面没有读取新帧的话就对这一帧进行处理
	for(vector<Rect>::iterator it=biaozhu_boxs.begin();it!=biaozhu_boxs.end();++it)
	{
		 cout<<"开始的时候哪有矩形要绘制"<<endl;
	     rectangle(img_drawing,(*it),Scalar(0,255,0));
		 outfile<<"1"<<" "
			 <<(*it).x<<" "<<(*it).y<<" "<<(*it).width<<" "<<(*it).height<<endl;
	}
	
	int frame_counter=1;
	while(1)
	{
		int c=waitKey(0);
		if( (c & 255) == 27 ){   //ESC键退出
			cout << "Exiting ...\n";
			break;
		}
		switch((char)c)
		{
			case 'n':
				//read the next frame
				++frame_counter;
				capture>>img_original;
				if(img_original.empty())
				{
					cout<<"\nVideo Finished!"<<endl;
					return 0;
				}

				img_original.copyTo(img_drawing);
				//save and draw all of the labeling rects
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
					outfile<<frame_counter<<" "
						    <<(*it).x<<" "<<(*it).y<<" "<<(*it).width<<" "<<(*it).height<<endl;
				}
				break;
			case 'z':
				//undo the latest labeling
				if(!biaozhu_boxs.empty())
				{
					vector<Rect>::iterator it_end=biaozhu_boxs.end();
					--it_end;
					biaozhu_boxs.erase(it_end);
				}
				img_original.copyTo(img_drawing);//把上次读帧弄过来，在新帧上不再显示
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
				}
				break;

			case 'c':
				//clear all the rects on the image
				biaozhu_boxs.clear();
				img_original.copyTo(img_drawing);
		}

	   imshow("Video",img_drawing);
	}

	return 0;
}