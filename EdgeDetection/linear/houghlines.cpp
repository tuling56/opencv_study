/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	houghlines.cpp
* Brief: Hough变换直线检测程序
* Source:\\samples2.4.10\cpp  官方例程，采用OpenCV2.0结构
* Status: 
* Date:	[3/13/2015 jmy]
************************************************************************/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout << "\nThis program demonstrates line finding with the Hough transform.\n"
            "Usage:\n"
            "./houghlines <image_name>, Default is pic1.png\n" << endl;
}

int stdHoughLine(int argc, char** argv)
{
    const char* filename = argc >= 2 ? argv[1] : "samples//swt.png";

    Mat src = imread(filename, 0);
    if(src.empty())
    {
        help();
        cout << "can not open " << filename << endl;
        return -1;
    }

    Mat dst, cdst;
    //Canny(src, dst, 50, 200, 3);
	threshold(src, dst, 20, 255, CV_THRESH_BINARY_INV);
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

#if 0
    vector<Vec2f> lines;
    HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 1, CV_AA);
    }
#else
    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, 50, 10, 2 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
    }
#endif
    imshow("source", src);
    imshow("detected lines", cdst);

    waitKey();

    return 0;
}


//功能测试区
//int main(int argc,char**argv)
//{
//	stdHoughLine(0, NULL);
//
//    std::cin.get();
//	return 0;
//}