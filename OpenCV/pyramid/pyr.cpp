/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	pyr.cpp
* Brief: 
* Source:
* Status: 
* Date:	[1/26/2015 jmy]
************************************************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int pyr(Mat&src)
{
	Mat pyrdown, pyrup, pyrbuild,diff,dst;
	pyrDown(src, pyrdown);
	pyrUp(src, pyrup);
	pyrUp(pyrdown, pyrbuild);
	//absdiff(src, pyrbuild(Range(0, src.rows), Range(0, src.cols)),diff);
	diff.create(src.size(), src.type());
	resize(pyrbuild, pyrbuild,src.size(),0,0,INTER_AREA); //调整重构后的图像使得其和原图像一样的大小
	absdiff(src, pyrbuild, diff);
	

	//新的应用:金字塔均值滤波，作为mean shift分割算法的初始化步骤
	//pyrMeanShiftFiltering()的使用，参考Detection/filtersmooth/meanshift_smooth_segmentaiton.cpp文件


	imshow("pyrdown", pyrdown);
	imshow("pyrup",pyrup);
	imshow("pyrbuild", pyrbuild);
	imshow("pydiff", diff); //结果显示重构的图像和原图像的差异很小
	
	waitKey();

	return 0;
}

//功能测试区
//int main()
//{
//	Mat src = imread("samples\\200.png");
//	pyr(src);
//
//	return 0;
//}