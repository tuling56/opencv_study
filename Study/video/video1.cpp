//Function：本代码为在视频帧中标记帧数，在第一帧鼠标取框的功能性测试程序
//Source:http://www.cnblogs.com/wsc36305/archive/2012/10/08/2715854.html
//Status:
//Improve:
//Info:[11/16/2014 jmy]


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace cv;
using namespace std;

Rect box; 
bool drawing_box = false;
bool gotBB = false;    
Mat frame;

void draw_framecount(Mat Frame,int framecount); 
void mouseHandler(int event, int x, int y, int flags, void *param);

int main12()
{
	//void mouseHandler(int event, int x, int y, int flags, void *param);
	namedWindow("text",CV_WINDOW_AUTOSIZE);
	
	//注册窗口鼠标事件
	cvSetMouseCallback("text",mouseHandler,NULL);
	
	Mat img;
	VideoCapture cap("PicVideo/video/test_o2.mp4");

	long count=0;
	for(;;)
	{   
		cap>>frame;
		rectangle(frame,box,Scalar(0,0,255));//在下一帧中仍画出上帧的选框
		draw_framecount(frame,count);  //绘制帧数
		
		//当视频中出现矩形框的时候，绘制矩形框
		if(gotBB)
		{
			rectangle(frame,box,Scalar(0,0,255));  //绘制矩形框选
			//imshow("text",frame);
			//waitKey(100);
			//在这里应该暂停一下，当按下空格键的时候再次开始
// 			char c=cvWaitKey();
// 			if (c==' ')
// 			{
// 				//继续下一帧
// 			}
		}

		imshow("text",frame);
		count++;
		//waitKey(100);

	}
	
	//释放鼠标事件
	setMouseCallback("text", NULL, NULL);

}

//标记当前帧的帧数
void draw_framecount(Mat Frame,int framecount)//Frame为传进来的当前帧，framecount为当前帧数
{
	char strFrame[10];
	sprintf(strFrame, "#%0d ",framecount);
	putText(Frame,strFrame,cvPoint(0,20),1,1,CV_RGB(25,200,25));
}

//鼠标事件的回调函数定义，用于在视频帧中截取一个矩形，可用于跟踪,回调函数是一个由调用方自己实现，供被调用方使用的特殊函数
//其中event 是 CV_EVENT_* 变量之一， x 和y 是鼠标指针在图像坐标系的坐标（不是窗口坐标系）， 
//flags 是CV_EVENT_FLAG 的组合， param 是用户定义的传递到cvSetMouseCallback 函数调用的参数。
void mouseHandler(int event, int x, int y, int flags, void *param)	
{
	switch (event)
	{
	case CV_EVENT_MOUSEMOVE:
		if (drawing_box)
		{
			box.width = x - box.x;
			box.height = y - box.y;
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		drawing_box = true;
		box = Rect(x, y, 0, 0);
		break;
	case CV_EVENT_LBUTTONUP:
		drawing_box = false;
		if (box.width < 0)
		{
			box.x += box.width;
			box.width *= -1;
		}
		if( box.height < 0 )
		{
			box.y += box.height;
			box.height *= -1;
		}
		gotBB = true;
		break;
	default:
		break;
	}
}