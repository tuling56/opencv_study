/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	watershed.cpp
* Brief: 基于分水岭的分割算法，
* Source:OpenCV自带例程
* Status: 
* Date:	[1/26/2015 jmy]
************************************************************************/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdio>
#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout << "\nThis program demonstrates the famous watershed segmentation algorithm in OpenCV: watershed()\n"
            "Usage:\n"
            "./watershed [image_name -- default is fruits.jpg]\n" << endl;


    cout << "Hot keys: \n"
        "\tESC - quit the program\n"
        "\tr - restore the original image\n"
        "\tw or SPACE - run watershed segmentation algorithm\n"
        "\t\t(before running it, *roughly* mark the areas to segment on the image)\n"
        "\t  (before that, roughly outline several markers on the image)\n";
}

//全局变量定义区
Mat markerMask, watershedimg;
Point prevPt(-1, -1);

static void onMouse( int event, int x, int y, int flags, void* )
{
    if( x < 0 || x >= watershedimg.cols || y < 0 || y >= watershedimg.rows ) //不在图像区域内
        return;
    if( event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON) ) 
        prevPt = Point(-1,-1);
    else if( event == CV_EVENT_LBUTTONDOWN )
        prevPt = Point(x,y);
    else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )
    {
        Point pt(x, y);
        if( prevPt.x < 0 ) //如果鼠标没有被按下过
            prevPt = pt;
        line( markerMask, prevPt, pt, Scalar::all(255), 5, 8, 0 ); //相当于把该曲线经过的地方连成了一个轮廓
        line( watershedimg, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        prevPt = pt;
        imshow("image", watershedimg);
		imshow("markerMask", markerMask);
    }
}

int mwatershed( int argc, char** argv )
//int main(int argc, char** argv)
{
    char* filename = argc >= 2 ? argv[1] : (char*)"samples\\grabCut.png";
    Mat img0 = imread(filename, 1), imgGray;

    if( img0.empty() ) {
        cout << "Couldn'g open image " << filename << ". Usage: watershed <image_name>\n";
        return 0;
    }
    help();
    namedWindow( "image", 1 );

    img0.copyTo(watershedimg);
    cvtColor(watershedimg, markerMask, COLOR_BGR2GRAY);
    cvtColor(markerMask, imgGray, COLOR_GRAY2BGR); //imgGray是伪彩图
    markerMask = Scalar::all(0);
    imshow( "image", watershedimg ); //是原始图像的一个备份
    setMouseCallback( "image", onMouse, 0 );

    for(;;)
    {
		int c = waitKey(0);
        if( (char)c == 27 )
            break;

        if( (char)c == 'r' )
        {
            markerMask = Scalar::all(0);
            img0.copyTo(watershedimg);
            imshow( "image", watershedimg );//把原图复制过去
        }

        if( (char)c == 'w' || (char)c == ' ' )
        {
            int i, j, compCount = 0;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
		    //注意是在markerMask上寻找轮廓
            findContours(markerMask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

            if( contours.empty() )
                continue;
            Mat markers(markerMask.size(), CV_32S);
            markers = Scalar::all(0);
            int idx = 0;
            for( ; idx >= 0; idx = hierarchy[idx][0], compCount++ ) //统计的是最外层的轮廓
                drawContours(markers, contours, idx, Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);
			imshow("markers", markers);

            if( compCount == 0 )
                continue;

			vector<Vec3b> colorTab;
            for( i = 0; i < compCount; i++ )
            {
                int b = theRNG().uniform(0, 255);
                int g = theRNG().uniform(0, 255);
                int r = theRNG().uniform(0, 255);
                colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
            }

            double t = (double)getTickCount();
            watershed( img0, markers );    //*核心*函数，markers是通过findCountour进行标记
            t = (double)getTickCount() - t;
            printf( "execution time = %gms\n", t*1000./getTickFrequency() );

            Mat wshed(markers.size(), CV_8UC3);

            // paint the watershed image
            for( i = 0; i < markers.rows; i++ )
			{ 
                for( j = 0; j < markers.cols; j++ )
                {
                    int index = markers.at<int>(i,j);
                    if( index == -1 )
                        wshed.at<Vec3b>(i,j) = Vec3b(255,255,255);
                    else if( index <= 0 || index > compCount )
                        wshed.at<Vec3b>(i,j) = Vec3b(0,0,0);
                    else
                        wshed.at<Vec3b>(i,j) = colorTab[index - 1];
                }
			}
            wshed = wshed*0.5 + imgGray*0.5;
            imshow( "watershed transform", wshed );
        }
    }

    return 0;
}

//功能测试区
//int main(int argc,char**argv)
//{
//	mwatershed(0, NULL);
//
//    std::cin.get();
//	return 0;
//}