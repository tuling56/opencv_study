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
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

// #pragma comment(lib, "cv.lib")
// #pragma comment(lib, "cxcore.lib")
// #pragma comment(lib, "highgui.lib")

int contour_test( int argc, char** argv )  
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
	cvGetQuadrangleSubPix(src,dst,&mat); //该函数的功能不明确
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
	//计算得到旋转矩阵----这里计算得到的矩阵不能使图像变换到想要的旋转结果
	//cv2DRotationMatrix(cvPoint2D32f(src->width*0.5f,src->height*0.5f),minRect.angle,0.6,rot);
	float m[6];
	CvMat mat=cvMat(2,3,CV_32FC1,m);
	float factor=1.0;//缩放因子
	float angle = -minRect.angle;
	float w=0,h=0;
	w=minRect.center.x; h=minRect.center.y;
	RotateImage(src,dst,cvPoint(w,h),angle,factor);//以中心为转
	//cvEllipseBox(dst,minRect,cvScalar(0,0,255));
	cvReleaseMat(&mat_contour);
	return minRect;//返回最佳包围盒
}


/*
 *	功能：图像旋转变换，保留原图内容，放大尺寸
 *  输入：
 *  输出：
 *  状态：
 */
//旋转图像内容不变，尺寸相应变大
IplImage* rotateMagnify(IplImage* img, int degree)
{
	double angle = degree  * CV_PI / 180.;
	double a = sin(angle), b = cos(angle);
	int width = img->width, height = img->height;
	//旋转后的新图尺寸
	int width_rotate = int(height * fabs(a) + width * fabs(b));
	int height_rotate = int(width * fabs(a) + height * fabs(b));
	IplImage* img_rotate = cvCreateImage(cvSize(width_rotate, height_rotate), img->depth, img->nChannels);
	cvZero(img_rotate);
	//保证原图可以任意角度旋转的最小尺寸
	int tempLength = sqrt((double)width * width + (double)height *height) + 10;
	int tempX = (tempLength + 1) / 2 - width / 2;
	int tempY = (tempLength + 1) / 2 - height / 2;
	IplImage* temp = cvCreateImage(cvSize(tempLength, tempLength), img->depth, img->nChannels);
	cvZero(temp);
	//将原图复制到临时图像tmp中心
	cvSetImageROI(temp, cvRect(tempX, tempY, width, height));
	cvCopy(img, temp, NULL);
	cvResetImageROI(temp);
	//旋转数组map
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]
	float m[6];
	int w = temp->width;
	int h = temp->height;
	m[0] = b;
	m[1] = a;
	m[3] = -m[1];
	m[4] = m[0];
	// 将旋转中心移至图像中间  
	m[2] = w * 0.5f;
	m[5] = h * 0.5f;
	CvMat M = cvMat(2, 3, CV_32F, m);
	cvGetQuadrangleSubPix(temp, img_rotate, &M);
	cvReleaseImage(&temp);
	return img_rotate;
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


/*
 *	功能：判断两个矩形是否相交，和重叠率
 *  输入：Rect r1,Rect r2
 *  输出：重叠率
 *  状态：
 */
float bbOverlap(Rect& box1,Rect& box2)
{
	if (box1.x > box2.x+box2.width) { return 0.0; }
	if (box1.y > box2.y+box2.height) { return 0.0; }
	if (box1.x+box1.width < box2.x) { return 0.0; }
	if (box1.y+box1.height < box2.y) { return 0.0; }

	float colInt =  min(box1.x+box1.width,box2.x+box2.width) - max(box1.x, box2.x);
	float rowInt =  min(box1.y+box1.height,box2.y+box2.height) - max(box1.y,box2.y);
	float intersection = colInt * rowInt;

	float area1 = box1.width*box1.height;
	float area2 = box2.width*box2.height;
	return intersection / (area1 + area2 - intersection);
}



//功能测试区
vector<Rect> rectlist;
int main()
{
	IplImage* srccolor = cvLoadImage("samples/box.png", CV_LOAD_IMAGE_COLOR);
	IplImage* src = cvLoadImage("samples/box.png", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	cvZero(dst);
	cvConvertScale(src,src,-1,255);
	
	cvNamedWindow("src", 1);
	cvNamedWindow("Bin", 1);
	cvNamedWindow("Result", 1);
	IplImage *pBinaryImage = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvThreshold(src, pBinaryImage, 150, 255, CV_THRESH_BINARY);
	cvShowImage("Bin", pBinaryImage);

	//-----------------------------------【轮廓查找】-----------------------------
	//描述：连通域提取
	//-----------------------------------------------------------------------
	CvSeq* contour = 0;
	CvMemStorage* cvMStorage = cvCreateMemStorage();
	int contournum =cvFindContours(pBinaryImage, cvMStorage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL);
	cout << "total contour:" << contournum << endl;

	//-----------------------------------【轮廓分析】-----------------------------
	//描述：分析和提取轮廓的形状，面积，宽高比等特征，连通域分析
	//-----------------------------------------------------------------------
	double maxarea = 0,double minarea = 100;
	for( ; contour != 0; contour = contour->h_next )  //同级别的下一个轮廓
	{
		cvDrawContours(dst, contour, CV_RGB(0, 255, 0), CV_RGB(0, 255, 0), -1, 1);//绘制轮廓

		//限制条件约束
		double length = cvArcLength(contour);//求轮廓长度
		double tmparea = fabs(cvContourArea(contour));//求面积
		//if (tmparea < minarea) {
		//	cvSeqRemove(contour, 0); // 删除面积小于设定值的轮廓
		//	continue;
		//}

		CvRect aRect = cvBoundingRect(contour, 0); //求外接矩形
		//if ((aRect.width / aRect.height) < 1){
		//	cvSeqRemove(contour, 0); //删除宽高比例小于设定值的轮廓
		//	continue;
		//}
		rectlist.push_back(aRect);
		cvRectangle(dst, cvPoint(aRect.x, aRect.y), cvPoint(aRect.x + aRect.width, aRect.y + aRect.height),
			        cvScalar(185, 103, 0), 1);

		CvBox2D box = cvMinAreaRect2(contour, NULL);//求最小外接矩形（关键是如何绘制倾斜矩形）
		CvPoint2D32f pt[4];
		cvBoxPoints(box, pt);
		for (int i = 0; i < 4; ++i){
			cvLine(srccolor, cvPointFrom32f(pt[i]), cvPointFrom32f(pt[((i + 1) % 4) ? (i + 1) : 0]), 
				   CV_RGB(255, 0, 0),2);
		}
		//最小外接的矩形的旋转，现在的问题时从contour到图像的转换，有图像之后才能旋转啊
		//待补充

	}
	printf("共有轮廓%d条\n", contournum);

	//-----------------------------------【轮廓融合】-----------------------------
	//描述：小轮廓融合成大轮廓和剔除
	//-----------------------------------------------------------------------
	Mat rectmerge(src);
	rectmerge.setTo(0);
	groupRectangles(rectlist,0);
	for (vector<Rect>::iterator it = rectlist.begin(); it != rectlist.end();it++){
		rectangle(rectmerge, *it, cvScalar(255, 0, 123), 1);
	}
	imshow("rectMerge", rectmerge);
	float overlaprate=0.0;
	Point2d tl,br;
	while (contournum--) 
	{
		for (vector<Rect>::iterator it1 = rectlist.begin(); it1 != rectlist.end();it1++)
		{
			for (vector<Rect>::iterator it2 = rectlist.begin(); it2 != rectlist.end();it2++)
			{
				overlaprate=bbOverlap(*it1,*it2);
				if (overlaprate<=0) continue; //不重叠
				tl=cvPoint(min(it1->x,it2->x),min(it1->y,it2->y));
				br=cvPoint(min(it1->x,it2->x),min(it1->y,it2->y));
			}
		}

	}

	//整理所有矩形,小矩形合并成大矩形
	BOOL temp_found=FALSE;//控制是否找到附近的矩形；
	int temp_textrows=0;
	for(int k=0;temp_textrows!=contournum;k++)//对矩形数据进行扫描的次数；
	{
		temp_textrows=contournum;//直到扫描后矩形个数不变为止；
		for (vector<Rect>::iterator it1 = rectlist.begin(); it1 != rectlist.end();it1++)
		{
			if(temp_found)
				it1--;
			temp_found=FALSE;
			for (vector<Rect>::iterator it2 = rectlist.begin(); it2 != rectlist.end();it2++)
			{

			
				
			}
		}
	}






	
	cvShowImage("src",srccolor);
	cvShowImage("Result",dst);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseImage(&pBinaryImage);
	

	return 0;
}

//功能测试区2
//int main()
//{
//	IplImage* src = cvLoadImage("samples/contour1.png", CV_LOAD_IMAGE_GRAYSCALE);
//	IplImage* dst = rotateMagnify(src,-50);
//	cvShowImage("dst", dst);
//	cvWaitKey(0);
//	return 0;
//}

