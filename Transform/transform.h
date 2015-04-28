/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	transform.cpp
* Brief: 变换头文件
* Source:
* Status: 
* Date:	[4/7/2015 jmy]
************************************************************************/
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

//-----------------------------------【仿射】-----------------------------
int handPerspective(std::string filename);

//-----------------------------------【透视】-----------------------------

//手动校正
int handPerspective(std::string filename);

//利用点对进行透视校正
int houghAutoRectify(cv::Mat src);
int rectAutoRectify(cv::Mat src);

//利用透视平面寻找变换公式
int homoPlaneAutoRectify();
