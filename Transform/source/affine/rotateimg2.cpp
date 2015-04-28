/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	rotateimg.cpp
* Brief: OpenCV2.0实现图像旋转和缩放
* Source:http://blog.csdn.net/masibuaa/article/details/9017761
* Status: 关键是进行了图像缩放，使得变换后的图像扩大了
* Date:	[3/28/2015 jmy]
************************************************************************/
#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define SCALE 1   //缩放比例

int rotate2()
{
	Mat src = imread("samples//rect.png");
	Mat dst;

	int angle = -30;//旋转角度(正值表示逆时针旋转)
	int length;//输出图像的宽度或高度，输出图像设置为正方形
	
	//但如果是缩小(SCALE<=1)，这样会导致临时图像中放不下原图，所以对于所有缩小的情况，输出图像和临时图像的长宽都设为原图的对角线长度
	if (SCALE <= 1)
		length = sqrt(src.cols*src.cols + src.rows*src.rows);
	else //为了保证无论如何旋转都能放下，输出图像的长和宽都设为输入图像对角线长度乘以SCALE
		length = sqrt(src.cols*src.cols + src.rows*src.rows) * SCALE;
	
	//建立临时图像，长宽都是图像的对角线长度，将源图像复制到临时图像的中心后再变换
	Mat tempImg(length, length, src.type(),Scalar(255,0,0));//临时图像，大小和输出图像一样大
	int ROI_x = length / 2 - src.cols / 2;//ROI矩形左上角的x坐标
	int ROI_y = length / 2 - src.rows / 2;//ROI矩形左上角的y坐标
	Rect ROIRect(ROI_x, ROI_y, src.cols, src.rows);//ROI矩形
	rectangle(tempImg, ROIRect, Scalar(0, 0, 255), 1);
	Mat tempImgROI2(tempImg, ROIRect);//tempImg的中间部分
	src.copyTo(tempImgROI2);//将原图复制到tempImg的中心

	Point2f center(length / 2, length / 2);//旋转中心
	circle(tempImg, center, 2, Scalar(0, 255, 0), 2);
	Mat M = getRotationMatrix2D(center, angle, SCALE);//计算旋转的仿射变换矩阵

	//输出看看算出的矩阵是什么
	cout << "变换矩阵：" << endl;
	cout << M.at<double>(0, 0) << "," << M.at<double>(0, 1) << "," << M.at<double>(0, 2) << "," << endl;
	cout << M.at<double>(1, 0) << "," << M.at<double>(1, 1) << "," << M.at<double>(1, 2) << "," << endl;

	warpAffine(tempImg, dst, M, Size(length, length));//仿射变换
	
	//截取目标图像
	Mat rst;
	getRectSubPix(dst, Size(src.rows, src.cols), center, rst);

	//显示
	imshow("src", src);
	imshow("tempImg", tempImg);
	imshow("dst", dst);
	imshow("rst", rst);

	waitKey(0);
	return 0;
}

//功能测试区
int main_(int argc,char**argv)
{
	rotate2();

    std::cin.get();
	return 0;
}