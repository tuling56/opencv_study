//Function： 轮廓的长度，面积，外接矩形(平行坐标轴),旋转外接矩形，外接圆,椭圆
//Source: http://blog.csdn.net/fdl19881/article/details/6730112
//Status: 轮廓形状描述子的计算，OpenCV1.0结构
//Improve:改写成Mat结构
//Info:[11/15/2014 jmy]

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>

using namespace std;

int shapeDescriptor1()
{
	IplImage *src = cvLoadImage("samples\\contour\\length_area.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	if (!src){
		printf("fail to load\n");
		return -1;
	}
	cvThreshold(src,src,50,255,CV_THRESH_BINARY); //必须先要进行二值化
	CvMemStorage *storage = cvCreateMemStorage();
	CvSeq *seq = NULL;
	int cnt = cvFindContours(src,storage,&seq);
	seq = seq->h_next;

	//以下的操作都是对单个轮廓的操作
	double length = cvArcLength(seq);       //求轮廓长
	double area = cvContourArea(seq);       //求轮廓面积
	CvRect rect = cvBoundingRect(seq,1);    //求轮廓的外接矩形
	CvBox2D box = cvMinAreaRect2(seq,NULL); //求轮廓的旋转外接矩形
	
	cout<<"Contour Length = "<<length<<endl;
	cout<<"Contour Area = "<<area<<endl;
	
	//外接矩形
	IplImage *dst = cvCreateImage(cvGetSize(src),8,3); cvZero(dst);
	cvDrawContours(dst,seq,CV_RGB(255,0,0),CV_RGB(255,0,0),0);
	cvRectangleR(dst,rect,CV_RGB(0,255,0));
	cvShowImage("dst",dst);
	cvWaitKey();

	//外接圆
	CvPoint2D32f center;
	float radius;
	cvMinEnclosingCircle(seq,&center,&radius);
	cvCircle(dst,cvPointFrom32f(center),cvRound(radius),CV_RGB(100,100,100));
	cvShowImage("dst",dst);
	cvWaitKey();

	//外接椭圆
	CvBox2D ellipse = cvFitEllipse2(seq);
	cvEllipseBox(dst,ellipse,CV_RGB(255,255,0));
	cvShowImage("dst",dst);
	cvWaitKey();

	//绘制旋转外接矩形
	CvPoint2D32f pt[4];
	cvBoxPoints(box,pt);
	for(int i = 0;i<4;++i){
		cvLine(dst,cvPointFrom32f(pt[i]),cvPointFrom32f(pt[((i+1)%4)?(i+1):0]),CV_RGB(0,0,255));
	}
	cvShowImage("dst",dst);
	
	cvWaitKey();

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseMemStorage(&storage);	

	return 0;
}


//功能测试区
//int main(int argc,char**argv)
//{
//	shapeDescriptor();
//
//	std::cin.get();
//	return 0;
//}