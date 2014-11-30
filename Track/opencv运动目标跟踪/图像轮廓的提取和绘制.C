#include "highgui.h"
#include "cv.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main(void)
{
    int i=0;
    int mode = CV_RETR_CCOMP;//设置提取轮廓模式
    int contours_num=0;//提取轮廓的数目
    CvScalar external_color;//绘制轮廓线的颜色
    CvScalar hole_color;
    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *contours =0;//存储提取的轮廓图像

    IplImage *pImg = NULL;
    IplImage *pContourImg = NULL;//显示提取的轮廓图像
    IplImage *src=/blog.armyourlife.info/cvLoadImage("1.png",1);

    pImg = cvCreateImage(cvGetSize(src),src->depth,1);
    pContourImg =cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,3);
    cvCvtColor(src,pImg,CV_BGR2GRAY);//将读取的图像转化为灰度图像
    cvNamedWindow("src",1);
    cvNamedWindow("contour",1);
    cvShowImage("src",src);//显示原图
    cvThreshold(pImg,pImg,200,255,CV_THRESH_BINARY);//二值化
    mode = CV_RETR_LIST;
    contours_num=cvFindContours(pImg,storage,&contours,sizeof(CvContour),mode,CV_CHAIN_APPROX_NONE);
    cout<<contours_num<<" "<<endl;//打印找到的轮廓的数目
    for (;contours!=0;contours=contours->h_next)
    {
        hole_color=CV_RGB(rand()&255,rand()&255,rand()&255);
        external_color = CV_RGB(rand()&255,rand()&255,rand()&255);
        cvDrawContours(pContourImg,contours,external_color,hole_color,1,2,8);
    }

    cvShowImage("contour",pContourImg);
    cvWaitKey(0);
    cvReleaseImage(&src);
    cvReleaseImage(&pImg);
    cvReleaseImage(&pContourImg);
    cvReleaseMemStorage(&storage);
} 



