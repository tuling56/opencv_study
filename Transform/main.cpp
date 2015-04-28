/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	main.cpp
* Brief: 图像变换的主入口，仿射和透视
* Source:
* Status: 
* Date:	[4/7/2015 jmy]
************************************************************************/
#include <stdio.h>
#include "transform.h"

using namespace cv;

int main_()
{
	Mat src = imread("samples\\ts.png");

	//-----------------------------------【仿射变换】-----------------------------
	//affine();
	
	//-----------------------------------【透视变换】-----------------------------
	houghAutoRectify(src);
		
	getchar();
	return 0;
}