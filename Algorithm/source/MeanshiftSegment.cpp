//参考：http://www.cnblogs.com/tornadomeet/archive/2012/06/06/2538695.html
// meanshift_segmentation.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;


Mat src,dst;
int spatialRad=10,colorRad=10,maxPryLevel=1;

//参数没传进去
void meanshift_seg(int,void*)
{
    pyrMeanShiftFiltering(src,dst,spatialRad,colorRad,maxPryLevel);//调用meanshift图像金字塔进行分割
	
    RNG rng=theRNG();
    Mat mask(dst.rows+2,dst.cols+2,CV_8UC1,Scalar::all(0));
    for(int i=0;i<dst.cols;i++)    //opencv图像等矩阵也是基于0索引的
        for(int j=0;j<dst.rows;j++)
            if(mask.at<uchar>(j+1,i+1)==0)
            {
                Scalar newcolor(rng(256),rng(256),rng(256));
                floodFill(dst,mask,Point(i,j),newcolor,0,Scalar::all(1),Scalar::all(1));
				// floodFill(dst,mask,Point(i,j),newcolor,0,colorDiff,colorDiff);
            }
	
    imshow("dst",dst);
}

//修改方法
//void meanshift_seg_s(int i,void*)
//{
//	spatialRad=i;
//	meanshift_seg();
//}
//
//void meanshift_seg_c(int i,void*)
//{
//	meanshift_seg();
//}
//
//void meanshift_seg_m(int i,void*)
//{
//	meanshift_seg();
//}




int MeanShiftSegment()
{
    
    namedWindow("src",1);
    namedWindow("dst",1);

    src=imread("test_a3(3).tif");
    CV_Assert(!src.empty());
	imshow("src",src);


    spatialRad=10;
    colorRad=10;
    maxPryLevel=1;

    //虽然createTrackbar函数的参数onChange函数要求其2个参数形式为onChange(int,void*)
    //但是这里是系统响应函数，在使用createTrackbar函数时，其调用的函数可以不用写参数，甚至
    //括号都不用写，但是其调用函数的实现过程中还是需要满足(int,void*)2个参数类型
    createTrackbar("spatialRad","dst",&spatialRad,20,meanshift_seg);
    createTrackbar("colorRad","dst",&colorRad,20,meanshift_seg);
    createTrackbar("maxPryLevel","dst",&maxPryLevel,5,meanshift_seg);

	//meanshift_seg_s(spatialRad,0);
	//meanshift_seg_c(colorRad,0);
	//meanshift_seg_m(maxPryLevel,0);

	meanshift_seg(0,0);

   /*
	char c=(char)waitKey();
    if(27==c) return 0;
   */
	imshow("dst",src);

	waitKey();//无限等待用户交互响应
	//while(1);//这里不能用while(1)的原因是需要等待用户的交互，而while(1)没有该功能。虽然2者都有无限等待的作用。
    return 0;
}