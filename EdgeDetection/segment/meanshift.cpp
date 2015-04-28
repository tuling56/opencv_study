//Function：MeanShift做图像平滑，然后利用漫水算法进行图像分割
//Source:OpenCV自带例程:http://www.cnblogs.com/tornadomeet/archive/2012/06/06/2538695.html
//Status:在空间金字塔中使用基于颜色的meanshift算法进行图像分割
//Improve:
//Info:[11/21/2014 jmy]

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>

// #pragma comment(lib, "opencv_core2410d.lib")  
// #pragma comment(lib, "opencv_highgui2410d.lib")  
// #pragma comment(lib, "opencv_imgproc2410d.lib") 
// #pragma comment(lib, "opencv_legacy2410d.lib") 

using namespace cv;
using namespace std;

//全局变量定义区
string winName = "meanshift";
int spatialRad, colorRad, maxPyrLevel;
Mat meanshiftsegment, res;


static void help(char** argv)
{
    cout << "\nDemonstrate mean-shift based color segmentation in spatial pyramid.\n"
    << "Call:\n   " << argv[0] << " image\n"
    << "This program allows you to set the spatial and color radius\n"
    << "of the mean shift window as well as the number of pyramid reduction levels explored\n"
    << endl;
}


//This colors the segmentations,分割结果的彩色显示,利用了漫水算法
static void floodFillPostprocess( Mat& img, const Scalar& colorDiff=Scalar::all(1) )
{
    CV_Assert( !img.empty() );
    RNG rng = theRNG();
    Mat mask( img.rows+2, img.cols+2, CV_8UC1, Scalar::all(0) ); //掩码
    for( int y = 0; y < img.rows; y++ )
    {
        for( int x = 0; x < img.cols; x++ )
        {
            if( mask.at<uchar>(y+1, x+1) == 0 )
            {
                Scalar newVal( rng(256), rng(256), rng(256) );
				//逐点漫水
                floodFill( img, mask, Point(x,y), newVal, 0, colorDiff, colorDiff );
            }
        }
    }
}


static void meanShiftSegmentation( int, void* )
{
    cout << "spatialRad=" << spatialRad << "; "
         << "colorRad=" << colorRad << "; "
         << "maxPyrLevel=" << maxPyrLevel << endl;
	
	//滤波部分，可以仔细参考下,其实里面也完成了分割
    pyrMeanShiftFiltering( meanshiftsegment, res, spatialRad, colorRad, maxPyrLevel ); 
	imshow("Res",res);

	//彩色Sobel边缘检测部分，分成平滑和非平滑的，做对比
	Mat imgsobel,imgsobel_no;
	Sobel(res,imgsobel,-1,1,1,3,1,0,BORDER_DEFAULT);  //res是平滑后的
	Sobel(meanshiftsegment,imgsobel_no,-1,1,1,3,1,0,BORDER_DEFAULT); //meanshifitsegment是原始没有平滑的 
	imshow("Smooth",imgsobel);
	imshow("NoSmooth",imgsobel_no);

	//显示分割结果
    floodFillPostprocess( res, Scalar::all(2) );
    imshow( winName, res );
}


int meanshift_smooth_segmentation(Mat src)
{
   /*
    if( argc !=2 ) {
        help(argv);
        return -1;
    }
   */
    if( src.empty() ){
        printf("can't load the img\n");
        getchar();
        return -1;
    }

    spatialRad = 10; //空间窗口搜索半径
    colorRad = 10;   //颜色窗口搜索半径
    maxPyrLevel = 1; //最大金字塔层数

    namedWindow( winName, CV_WINDOW_AUTOSIZE );

    createTrackbar( "spatialRad", winName, &spatialRad, 80, meanShiftSegmentation );
    createTrackbar( "colorRad", winName, &colorRad, 60, meanShiftSegmentation );
    createTrackbar( "maxPyrLevel", winName, &maxPyrLevel, 5, meanShiftSegmentation );

    meanShiftSegmentation(0, 0);
    waitKey();
    return 0;
}

//功能测试区
//int main(int argc, char**argv)
//{
//	meanshiftsegment = imread("samples\\segment\\meanshift.jpg");
//	meanshift_smooth_segmentation(meanshiftsegment);
//	std::cin.get();
//	return 0;
//}