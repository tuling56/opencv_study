/************************************************************************
* Copyright(c) 2014 tuling56
*
* File:	compare_img.cpp
* Brief: 利用opencv做图片的相似度比较
* Source:http://blog.csdn.net/rcfalcon/article/details/7718436
* Status: 
* Date:	[11/26/2014 jmy]
************************************************************************/

#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;

CvSeq* getImageContour(IplImage* srcIn) 
{
	IplImage* src;
	src = cvCreateImage(cvGetSize(srcIn),8,1);

	//拷贝图像 
	cvCopy(srcIn,src);
	//创建空间 
	CvMemStorage* mem = cvCreateMemStorage(0);
	//结构体 (mem和seq属于同一个地址<当释放的时候只要释放一个就行了>)
	CvSeq* seq;

	if(!mem){
		printf("mem is null");
	}

	//二值化图像
	cvThreshold(src,src,100,255,0);
	//计算图像轮廓 计算后的轮廓信息不能释放 在后面需要用到
	cvFindContours(src,mem,&seq,sizeof(CvContour),CV_RETR_CCOMP);
	//释放图像空间
	cvReleaseImage(&src);
	//返回轮廓信息
	return seq;
}


int main_comp(int argc, char* argv[]){

	//加载图像1
	IplImage* src1 = cvLoadImage("samples\\7.png",CV_LOAD_IMAGE_GRAYSCALE);
	//获取图像1的轮廓
	CvSeq* contour1;
	contour1 = getImageContour(src1);


	//加载图像2
	IplImage* src2 = cvLoadImage("samples\\2.png",CV_LOAD_IMAGE_GRAYSCALE);
	//获取图像2的轮廓
	CvSeq* contour2;
	contour2 = getImageContour(src2);


	//进行比对  -- 比较这两个轮廓所对应的HU矩的距离 有多远
	double result;
	//cvMatchShapes函数参数（轮廓1，轮廓2，方法）
	//计算第一个轮廓的普通矩和中心距 ， 然后计算这个矩形的HU矩（HU距的7个数值保存在ma的数组里）
	//的哥个轮廓同上(HU距保存在mb中) 
	//然后比较这两个HU距的相似性 
	//方法参数有3个 参数分别是1,2,3 三种计算方法
	result = cvMatchShapes(contour1,contour2,1);

	//输出结果 结果越小 相似度越高
	printf("result is %f \n",result);

	//释放轮廓存储空间
	cvReleaseMemStorage(&contour1->storage);
	cvReleaseMemStorage(&contour2->storage);

	//释放图像空间
	cvReleaseImage(&src1);
	cvReleaseImage(&src2);

	getchar();

	return 0;

}


/*****************************************************/
/* 用Mat改写的方法                                     */
/*****************************************************/

Mat getImageContour(Mat srcIn)
{

	Mat src(srcIn),seq;
	//二值化图像
	threshold(src,src,100,255,0);
	//计算图像轮廓 计算后的轮廓信息不能释放 在后面需要用到
	findContours(src,seq,sizeof(CvContour),CV_RETR_CCOMP);
	//返回轮廓信息
	return seq;

}


int main_amt(int argc, char* argv[])
{

	//加载图像1
	Mat src1 = imread("samples\\7.png",CV_LOAD_IMAGE_GRAYSCALE);
	Mat contour1 = getImageContour(src1);

	//加载图像2
	Mat src2 = imread("samples\\7.png",CV_LOAD_IMAGE_GRAYSCALE);
	Mat contour2 = getImageContour(src1);


	//进行比对  -- 比较这两个轮廓所对应的HU矩的距离 有多远
	double result;
	//cvMatchShapes函数参数（轮廓1，轮廓2，方法）
	//计算第一个轮廓的普通矩和中心距 ， 然后计算这个矩形的HU矩（HU距的7个数值保存在ma的数组里）
	//的哥个轮廓同上(HU距保存在mb中) 
	//然后比较这两个HU距的相似性 
	//方法参数有3个 参数分别是1,2,3 三种计算方法
 	result = matchShapes(contour1,contour2,1,0.0);

	//输出结果 结果越小 相似度越高
	printf("result is %f \n",result);

	getchar();

	return 0;

}
