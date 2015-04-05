/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	maxMinFilter.cpp
* Brief: 最值滤波，非最值抑制
* Source:http://blog.csdn.net/quarryman/article/details/9732349
* Status: 
* Date:	[2/6/2015 jmy]
************************************************************************/

#include <cv.h>
#include <highgui.h>

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

CvRect kcvRectIntersection(CvRect rect1, CvRect rect2)
{
	CvRect rect;
	rect.x = max(rect1.x, rect2.x);
	rect.y = max(rect1.y, rect2.y);
	rect.width = min(rect1.x + rect1.width, rect2.x + rect2.width);
	rect.width = rect.width - rect.x;
	rect.height = min(rect1.y + rect1.height, rect2.y + rect2.height);
	rect.height = rect.height - rect.y;
	return rect;
}

CvRect kcvGetRectFromCenterAndSize(int cx, int cy, int w, int h = 0)
{
	CvRect rect;
	h = (h == 0 ? w : h);
	rect.x = cx - (w >> 1);
	rect.y = cy - (h >> 1);
	rect.width = w;
	rect.height = h;
	return rect;
}

int minValue(IplImage* img, CvRect rect)
{
	uchar minval = 255;
	for (int i = rect.y; i < rect.y + rect.height; ++i)
	{
		for (int j = rect.x; j < rect.x + rect.width; ++j)
		{
			if (CV_IMAGE_ELEM(img, uchar, i, j) < minval)
			{
				minval = CV_IMAGE_ELEM(img, uchar, i, j);
			}
		}
	}
	return minval;
}

int maxValue(IplImage* img, CvRect rect)
{
	uchar maxval = 0;
	for (int i = rect.y; i < rect.y + rect.height; ++i)
	{
		for (int j = rect.x; j < rect.x + rect.width; ++j)
		{
			if (CV_IMAGE_ELEM(img, uchar, i, j) > maxval)
			{
				maxval = CV_IMAGE_ELEM(img, uchar, i, j);
			}
		}
	}
	return maxval;
}

enum
{
	KCV_MAX,		// 最大值滤波器，类似于形态学膨胀
	KCV_MIN,		// 最小值滤波器，类似于形态学腐蚀
	KCV_NMS_MAX,	// 非最大值抑制
	KCV_NMS_MIN		// 非最小值抑制
};

void maxminFilter(IplImage* src, IplImage* dst, int width, int height = 0, int mode = KCV_MAX)
{
	for (int j = 0; j < src->width; ++j)
	{
		for (int i = 0; i < src->height; ++i)
		{
			CvRect rect1 = cvRect(0, 0, src->width, src->height);
			CvRect rect2 = kcvGetRectFromCenterAndSize(j, i, width, height);
			CvRect rect = kcvRectIntersection(rect1, rect2);
			switch (mode)
			{
			case KCV_MAX:
				CV_IMAGE_ELEM(dst, uchar, i, j) = maxValue(src, rect);//注意只是访问了src,而修改的是在dst
				break;
			case KCV_MIN:
				CV_IMAGE_ELEM(dst, uchar, i, j) = minValue(src, rect);
				break;
			case KCV_NMS_MAX:
				if (CV_IMAGE_ELEM(src, uchar, i, j) != maxValue(src, rect))
				{
					CV_IMAGE_ELEM(dst, uchar, i, j) = 0;
				}
				else
				{
					CV_IMAGE_ELEM(dst, uchar, i, j) = CV_IMAGE_ELEM(src, uchar, i, j);
				}
				break;
			case KCV_NMS_MIN:
				if (CV_IMAGE_ELEM(src, uchar, i, j) != minValue(src, rect))
				{
					CV_IMAGE_ELEM(dst, uchar, i, j) = 255;
				}
				else
				{
					CV_IMAGE_ELEM(dst, uchar, i, j) = CV_IMAGE_ELEM(src, uchar, i, j);
				}
				break;
			}
		}
	}
}

//功能测试区
//int main()
//{
//	IplImage* src = cvLoadImage("samples\\swt.png", 0);
//	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1);//在这里可以看到dst是空图像
//	cvNamedWindow("original image");
//	cvShowImage("original image", src);
//
//	maxminFilter(src, dst, 3, 3, KCV_MAX);
//	cvNamedWindow("maximum filter");
//	cvShowImage("maximum filter", dst);
//	cvSaveImage("maximum filter.jpg", dst);
//
//	maxminFilter(src, dst, 3, 3, KCV_MIN);
//	cvNamedWindow("minimum filter");
//	cvShowImage("minimum filter", dst);
//	cvSaveImage("minimum filter.jpg", dst);
//
//	maxminFilter(src, dst, 3, 3, KCV_NMS_MAX);
//	cvNamedWindow("non-maximum suppression");
//	cvShowImage("non-maximum suppression", dst);
//	cvSaveImage("non-maximum suppression.jpg", dst);
//
//	maxminFilter(src, dst, 3, 3, KCV_NMS_MIN);
//	cvNamedWindow("non-minimum suppression");
//	cvShowImage("non-minimum suppression", dst);
//	cvSaveImage("non-minimum suppression.jpg", dst);
//
//	cvWaitKey(0);
//	cvDestroyAllWindows();
//	cvReleaseImage(&src);
//	cvReleaseImage(&dst);
//
//	return 0;
//}