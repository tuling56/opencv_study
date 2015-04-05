//Function： 轮廓级别的学习
//Source:http://blog.csdn.net/augusdi/article/details/9000893
//Status: 改变的只是轮廓的绘制等级
//Improve:
//Info:[11/19/2014 jmy]

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include <math.h>
 

#define w 500
int g11_levels = 3;
CvSeq* contours = 0;
 
void on_contourtrackbar(int pos)
{
    IplImage* cnt_img = cvCreateImage( cvSize(w,w), 8, 3 );
    CvSeq* _contours = contours;
    int _levels = g11_levels - 3;
	
	// get to the nearest face to make it look more funny
    if( _levels <= 0 ) 
        _contours = _contours->h_next->h_next->h_next->h_next->h_next->h_next->h_next->v_next->h_next->h_next;
    //_contours = _contours->v_next;
    
	cvZero( cnt_img );
    cvDrawContours( cnt_img, _contours, CV_RGB(255,0,0), CV_RGB(0,255,0), _levels);//, 3, CV_AA, cvPoint(0,0) );

	/*_levels:
	3，所有外轮廓及包含的内轮廓及里面的内轮廓
	2：所有外轮廓及包含的内轮廓
	1:所有外轮廓
	0：找到的第一个外轮廓
	-1：第一个外轮廓及包含的内轮廓
	-2：第一个外轮廓及包含的内轮廓及里面的内轮廓


	_contours->h_next:同级的下一个轮廓
	_contours->v_next:父级下的下层区域；
	*/
    cvShowImage( "contours", cnt_img );
    cvReleaseImage( &cnt_img );
}
 
int contoursg11()
{
    CvMemStorage* storage = cvCreateMemStorage(0);

	//绘制笑脸图
    IplImage* img = cvCreateImage( cvSize(w,w), 8, 1 );
    cvZero( img );
    for(int  i=0; i < 6; i++ )
    {
        int dx = (i%2)*250 - 30;//0%2=0;
        int dy = (i/2)*150;
        CvScalar white = cvRealScalar(255);
        CvScalar black = cvRealScalar(0);
        if( i == 0 )
        {
            for(int j = 0; j <= 10; j++ )
            {
                double angle = (j+5)*CV_PI/21;
                cvLine(img, cvPoint(cvRound(dx+100+j*10-80*cos(angle)),
                    cvRound(dy+100-90*sin(angle))),
                    cvPoint(cvRound(dx+100+j*10-30*cos(angle)),
                    cvRound(dy+100-30*sin(angle))), white, 1, 8, 0);
            }
        }
 
        cvEllipse( img, cvPoint(dx+150, dy+100), cvSize(100,70), 0, 0, 360, white, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+115, dy+70), cvSize(30,20), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+185, dy+70), cvSize(30,20), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+115, dy+70), cvSize(15,15), 0, 0, 360, white, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+185, dy+70), cvSize(15,15), 0, 0, 360, white, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+115, dy+70), cvSize(5,5), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+185, dy+70), cvSize(5,5), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+150, dy+100), cvSize(10,5), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+150, dy+150), cvSize(40,10), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+27, dy+100), cvSize(20,35), 0, 0, 360, white, -1, 8, 0 );
        cvEllipse( img, cvPoint(dx+273, dy+100), cvSize(20,35), 0, 0, 360, white, -1, 8, 0 );
    }
 
    cvNamedWindow( "image", 1 );
    cvShowImage( "image", img );

    //寻找轮廓
    cvFindContours( img, storage, &contours, sizeof(CvContour),2, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
 
    //comment this out if you do not want approximation
	//cvApproxPoly: 逼近方法 精度 逼近曲线是否封闭
    //contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1 ); 
   
    cvNamedWindow( "contours", 1 );
    cvCreateTrackbar( "levels+3", "contours", &g11_levels, 7, on_contourtrackbar );
    on_contourtrackbar(0);
    
	cvWaitKey(0);
    cvReleaseMemStorage( &storage );
    cvReleaseImage( &img );
 
    return 0;
}

//功能测试区
//int main(int argc,char**argv)
//{
//	contoursg11();
//
//    std::cin.get();
//	return 0;
//}