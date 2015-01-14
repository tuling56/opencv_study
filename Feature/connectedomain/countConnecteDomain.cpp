/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	countConnecteDomain.cpp
* Brief: OpenCV计算连通域的数目，并标出最大连通域
* Source:http://blog.csdn.net/augusdi/article/details/9008921
* Status: 连通域分析目前还利用轮廓检测的方法，参考Detection->contour目录
* Date:	[1/14/2015 jmy]
************************************************************************/

#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

// #pragma comment(lib, "cv.lib")
// #pragma comment(lib, "cxcore.lib")
// #pragma comment(lib, "highgui.lib")

int main1( int argc, char** argv )  
{
	IplImage* srccolor = cvLoadImage("samples/contour1.png", CV_LOAD_IMAGE_COLOR);
	IplImage* src = cvLoadImage("samples/contour1.png", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;
	cvThreshold(src, src,120, 255, CV_THRESH_BINARY);	// 二值化
	cvNamedWindow("Source", 1);
	cvShowImage("Source", src);
	
	// 提取轮廓
	int contour_num = cvFindContours(src, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	cvZero(dst);  // 清空数组
	CvSeq *_contour = contour; 
	double maxarea = 0;
	double minarea = 100;

	int n=0,m = 0;
	for( ; contour != 0; contour = contour->h_next )  //同级别的下一个轮廓
	{  
		n++;
		double tmparea = fabs(cvContourArea(contour));//求面积
		CvRect aRect = cvBoundingRect( contour, 0 ); //求外接矩形
		cvRectangle(dst,cvPoint(aRect.x,aRect.y),cvPoint(aRect.x+aRect.width,aRect.y+aRect.height),cvScalar(185,103,0),2);
		//cvMinAreaRect()
		if(tmparea < minarea) {  
			cvSeqRemove(contour, 0); // 删除面积小于设定值的轮廓
			continue;
		}  
		
		if ((aRect.width/aRect.height)<1){  
			cvSeqRemove(contour, 0); //删除宽高比例小于设定值的轮廓
			continue;
		}  
		if(tmparea > maxarea){  
			maxarea = tmparea;
		}  
		m++;
		// 创建一个色彩值
		CvScalar extern_color = CV_RGB( 255, 0, 0 );//外轮廓-红
		CvScalar inner_color = CV_RGB( 0, 255 , 0 );//内轮廓-绿
		
		//max_level 绘制轮廓的最大等级。如果等级为0，绘制单独的轮廓。如果为1，绘制轮廓及在其后的相同的级别下轮廓
		//如果值为2，所有的轮廓。如果等级为2，绘制所有同级轮廓及所有低一级轮廓，诸此种种
		//如果值为负数，函数不绘制同级轮廓，但会升序绘制直到级别为abs(max_level)-1的子轮廓
		cvDrawContours(dst, contour, extern_color,inner_color, -1, 2, 8);	//绘制外部和内部的轮廓
	}  
	printf("共有%d;满足条件的矩形有%d\n",n,m);

	//------------------------------【求最大轮廓】-----------------------------
	//描述：
	//-----------------------------------------------------------------------
	//contour = _contour;
	//int count = 0;
	//for(; contour != 0; contour = contour->h_next)
	//{  
	//	count++;
	//	//CvRect aRect = cvBoundingRect( contour, 0 ); //求外接矩形
	//	//cvRectangle(dst,cvPoint(aRect.x,aRect.y),cvPoint(aRect.x+aRect.width,aRect.y+aRect.height),cvScalar(255,0,0),1);
	//	double tmparea = fabs(cvContourArea(contour));
	//	if (tmparea == maxarea)  
	//	{  
	//		CvScalar color = CV_RGB( 211, 54, 206);
	//		cvDrawContours(dst, contour, color, color, -1, 2, 8);//绘制最大轮廓的外接矩形
	//	}  
	//}  
	//printf("The total number of contours is:%d", count);

	cvSaveImage("or.png",dst);
	cvAdd(srccolor,dst,dst);

	cvNamedWindow("Components", 1);
	cvShowImage("Components", dst);
	cvWaitKey(0);
	cvDestroyWindow("Source");
	cvReleaseImage(&src);
	cvDestroyWindow("Components");
	cvReleaseImage(&dst);

	return 0;
}  
 

/*
 *	功能：图像旋转，以点center为旋转中心，对src旋转angle度并缩放factor倍
 *  输入：
 *  输出：
 *  状态：
 */
void   RotateImage(IplImage *src,IplImage *dst,CvPoint center,float angle,float factor)
{
	float m[6];
	CvMat mat=cvMat(2,3,CV_32FC1,m);
	m[0] = (float)(factor*cos(-angle*CV_PI/180.));
	m[1] = (float)(factor*sin(-angle*CV_PI/180.));
	m[2] = center.x;
	m[3] = -m[1];
	m[4] = m[0];
	m[5] = center.y;
	cvSetZero(dst);
	cvGetQuadrangleSubPix(src,dst,&mat);
}

/*
 *	功能：传进来一个contour,然后计算它的最小包围矩形minRect，再把原图以包围矩形中心为旋转中心旋转minRect.angle°，得到调正的图像。
 *  输入：
 *  输出：dst 是通过cvClone()src得到的
 *  状态：
 */
CvBox2D RegionRotate(IplImage *src,IplImage *dst,CvSeq *contour)
{
	CvMat *mat_contour = cvCreateMat(1,contour->total,CV_32FC2);//双通道
	CvPoint2D32f *ptr_mat=(CvPoint2D32f*)(mat_contour->data.ptr);
	for (int i=0;i!=contour->total;++i)
	{
		CvPoint *ptr_seq=(CvPoint*)(cvGetSeqElem(contour,i));
		*ptr_mat=cvPointTo32f(*ptr_seq);//显示把CvPoint转换成CvPoint2D32F
		ptr_mat++;
	}//把轮廓变成矩阵
	CvBox2D minRect = cvMinAreaRect2(mat_contour);//得到最小包围矩形
	//CvMat *rot = cvCreateMat(2,3,CV_32FC1);
	//cv2DRotationMatrix(cvPoint2D32f(src->width*0.5f,src->height*0.5f),minRect.angle,0.6,rot);//计算得到旋转矩阵----这里计算得到的矩阵不能使图像变换到想要的旋转结果
	float m[6];
	CvMat mat=cvMat(2,3,CV_32FC1,m);
	float factor=1.0;//缩放
	float angle = -minRect.angle;
	float w=0,h=0;
	w=minRect.center.x; h=minRect.center.y;
	RotateImage(src,dst,cvPoint(w,h),angle,factor);
	//cvEllipseBox(dst,minRect,cvScalar(0,0,255));
	cvReleaseMat(&mat_contour);
	return minRect;//返回最佳包围盒
}

/*
 *	功能：在给定的contour中找到面积最大的一个轮廓，并返回指向该轮廓的指针
 *  输入：
 *  输出：
 *  状态：
 */

CvSeq *GetAreaMaxContour(CvSeq *contour)
{
	double contour_area_temp=0,contour_area_max=0;
	CvSeq * area_max_contour = 0 ;//指向面积最大的轮廓
	CvSeq* c=0;
	//printf( "Total Contours Detected: %d/n", Nc );
	for(c=contour; c!=NULL; c=c->h_next )
	{//寻找面积最大的轮廓，即循环结束时的area_max_contour
		contour_area_temp = fabs(cvContourArea( c, CV_WHOLE_SEQ )); //获取当前轮廓面积
		if( contour_area_temp > contour_area_max )
		{
			contour_area_max = contour_area_temp; //找到面积最大的轮廓
			area_max_contour = c;//记录面积最大的轮廓
		}
	}

	return area_max_contour;
}

//功能测试区
int main()
{
	IplImage* src = cvLoadImage("samples/contour1.png", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	cvZero(dst);
	
	IplImage *pBinaryImage = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvThreshold(src, src, 50, 255, CV_THRESH_BINARY);
	cvShowImage("Bin", src);

	CvSeq* contour = 0;
	CvMemStorage* cvMStorage = cvCreateMemStorage();
	// 检索轮廓并返回检测到的轮廓的个数
	cvFindContours(pBinaryImage,cvMStorage, &contour,sizeof(CvContour),CV_RETR_EXTERNAL);
	for( ; contour != 0; contour = contour->h_next )  //同级别的下一个轮廓
	{  
		cvDrawContours(dst, contour, CV_RGB(255,0,0), CV_RGB(0,255,0),-1);
	}
	cvSaveImage("Result",dst);
	cvWaitKey(0);

	return 0;
}