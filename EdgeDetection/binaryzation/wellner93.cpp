/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	wellner93.cpp
* Brief: 
* Reference:http://blog.csdn.net/xgmiao/article/details/9320667
* Status: 
* Date:	[5/7/2015 jmy]
************************************************************************/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void quickAdaptiveThreshold(unsigned char* grayImage, unsigned char* binImage, int width, int height)
{

	int t = 15;
	int s = width >> 3;
	const int T = 9; //T是避免浮点数运算
	const int power2S = 1 << T; //同时乘除2的T次方,避免浮点运算

	int factor = power2S * (100 - t) / (100 * s);
	int gn = 127 * s;
	int q = power2S - power2S / s; //等比数列比率
	int pn, hn;
	unsigned char *scanline = NULL;
	int *prev_gn = NULL;

	prev_gn = new int[width];
	for (int i = 0; i < width; i++)
	{
		prev_gn[i] = gn;
	}

	//左右交替扫描所有行
	for (int y = 0; y < height; y++)
	{
		int yh = y * width;
		scanline = grayImage + y * width;
		for (int x = 0; x < width; x++)    //该行从左往右扫描
		{
			pn = scanline[x];
			gn = ((gn * q) >> T) + pn;
			hn = (gn + prev_gn[x]) >> 1;     //上下两行求平均
			prev_gn[x] = gn;
			pn < (hn*factor) >> T ? binImage[yh + x] = 0 : binImage[yh + x] = 0xff;
		}
		y++;
		if (y == height)
		{
			break;
		}
		yh = y * width;
		scanline = grayImage + y * width;
		for (int x = width - 1; x >= 0; x--)  //该行从右往左扫描
		{
			pn = scanline[x];
			gn = ((gn * q) >> T) + pn;
			hn = (gn + prev_gn[x]) >> 1;
			prev_gn[x] = gn;
			pn < (hn*factor) >> T ? binImage[yh + x] = 0 : binImage[yh + x] = 0xff;
		}
	}

	delete prev_gn;
}


//功能测试区
//int main(int argc,char**argv)
//{
//	Mat gray = imread("E:\\360YPan\\samples\\textdete\\137.jpg", 0);
//	Mat bin;
//	gray.copyTo(bin);
//
//	quickAdaptiveThreshold(gray.data, bin.data, gray.cols, gray.rows);
//	imshow("Bin", bin);
//	waitKey();
//
//	return 0;
//}