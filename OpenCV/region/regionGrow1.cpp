/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	regionGrow1.cpp
* Brief: 
* Source:http://blog.sina.com.cn/s/blog_5e0d222e0100kbju.html
* Status: 未完成
* Date:	[1/23/2015 jmy]
************************************************************************/

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stack> //标准命名空间里的Stack

using namespace std;

typedef struct SeedPoint//保存种子像素
{
	int x;
	int y;
}seedpoint;

void Grow(IplImage* src, IplImage* seed, int gray)//gray=255
{
	stack<seedpoint> seedS;
	seedpoint point;
	// 获取图像数据,保存种子区域
	int height = seed->height;
	int width = seed->width;
	int step = seed->widthStep;
	int channels = seed->nChannels;
	uchar* seed_data = (uchar *)seed->imageData;
	uchar* src_data = (uchar *)src->imageData;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (seed_data[i*step + j] == 255)
			{
				point.x = i;
				point.y = j;
				seedS.push(point);
			}
		}
	}
	while (!seedS.empty())
	{
		seedpoint temppoint;
		point = seedS.top();
		seedS.pop();
		if ((point.x>0) && (point.x < (height - 1)) && (point.y>0) && (point.y < (width - 1)))
		{
			if ((seed_data[(point.x - 1)*step + point.y] == 0) && (src_data[(point.x - 1)*step + point.y] == gray))
			{
				seed_data[(point.x - 1)*step + point.y] = 255;
				temppoint.x = point.x - 1;
				temppoint.y = point.y;
				seedS.push(temppoint);
			}
			if ((seed_data[point.x*step + point.y + 1] == 0) && (src_data[point.x*step + point.y + 1] == gray))
			{
				seed_data[point.x*step + point.y + 1] = 255;
				temppoint.x = point.x;
				temppoint.y = point.y + 1;
				seedS.push(temppoint);
			}
			if ((seed_data[point.x*step + point.y - 1] == 0) && (src_data[point.x*step + point.y - 1] == gray))
			{
				seed_data[point.x*step + point.y - 1] = 255;
				temppoint.x = point.x;
				temppoint.y = point.y - 1;
				seedS.push(temppoint);
			}
			if ((seed_data[(point.x + 1)*step + point.y] == 0) && (src_data[(point.x + 1)*step + point.y] == gray))
			{
				seed_data[(point.x + 1)*step + point.y] = 255;
				temppoint.x = point.x + 1;
				temppoint.y = point.y;
				seedS.push(temppoint);
			}
		}
	}
}