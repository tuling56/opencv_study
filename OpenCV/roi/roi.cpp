//该函数的功能是截取原始图像的部分区域保存为一幅新图像

#include <cv.h>
#include <highgui.h>
#include "../opencv.h"


int ROIimg(char*filename)
{
	 IplImage *pSrc = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
	 if (!pSrc)
	 {
		printf("加载失败！");
		return 0;
	 }
	 cvNamedWindow("SRC",1);
	 cvShowImage("SRC",pSrc);

	 CvSize ROISize=cvSize(30,40);
	 cvSetImageROI(pSrc,cvRect(0,0,ROISize.width,ROISize.height));//设置原图像ROI
	 
	 
	 IplImage* pDest = cvCreateImage(ROISize,pSrc->depth,pSrc->nChannels);//创建目标图像,大小为ROI的大小
	 cvCopy(pSrc,pDest); //复制图像
	 cvResetImageROI(pSrc);//源图像用完后，清空ROI
	 
	 cvNamedWindow("ROI",1);
	 cvShowImage("ROI",pDest);
	 cvSaveImage("PicVideo/Roi.jpg",pDest);//保存目标图像
	 
	 color_cluster("PicVideo/Roi.jpg",2);
	 cvWaitKey(0);

}


