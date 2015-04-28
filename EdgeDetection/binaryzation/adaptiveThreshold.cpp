/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	AdaptiveFindThreshold.cpp
* Brief: 自适应二值化，给出上下阈值
* Source:
* Status: 
* Date:	[3/28/2015 jmy]
************************************************************************/

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

/*
 * Function:自适应计算灰度图像二值化的高低阈值
 * Input:
 *		X方向的梯度图
 *		Y方向的梯度图
 * Output:
 *		高低阈值【以指针参数的形式传出】
 */

void adaptiveThreshold(IplImage *Gradientdx, IplImage *Gradientdy, double *low, double *high)  
{  
	//IplImage转化为CvMat
	CvMat* dx=cvCreateMat(Gradientdx->height,Gradientdx->width,CV_32FC1);
	CvMat* dy=cvCreateMat(Gradientdy->height,Gradientdy->width,CV_32FC1);
	cvConvert(Gradientdx,dx);
	cvConvert(Gradientdx,dy);
	
	CvSize size=cvGetSize(Gradientdx);  
	IplImage *imge=0;  
	int i,j;  
	CvHistogram *hist;  
	int hist_size = 255;  
	float range_0[]={0,256};  
	float* ranges[] = { range_0 };  
	double  PercentOfPixelsNotEdges = 0.7;  
	
	imge = cvCreateImage(size, IPL_DEPTH_32F, 1);  
	
	// 计算边缘的强度, 并存于图像中  
	float maxv = 0;  
	for(i = 0; i < size.height; i++ )  
	{  
		const short* _dx = (short*)(dx->data.ptr + dx->step*i);  
		const short* _dy = (short*)(dy->data.ptr + dy->step*i);  
		float* _image = (float *)(imge->imageData + imge->widthStep*i);  
		for(j = 0; j < size.width; j++)  
		{  
			_image[j] = (float)(abs(_dx[j]) + abs(_dy[j]));  
			maxv = maxv < _image[j] ? _image[j]: maxv;  
		}  
	}  

	// 计算直方图  
	range_0[1] = maxv;  
	hist_size = (int)(hist_size > maxv ? maxv:hist_size);  
	hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);  
	cvCalcHist( &imge, hist, 0, NULL );  
	int total = (int)(size.height * size.width * PercentOfPixelsNotEdges);  
	float sum=0;  
	int icount = hist->mat.dim[0].size;  

	float *h = (float*)cvPtr1D( hist->bins, 0 );  
	for(i = 0; i < icount; i++)  
	{  
		sum += h[i];  
		if( sum > total )  
			break;   
	}  
	
	// 计算高低门限  
	*high = (i+1) * maxv / hist_size ;  
	*low = *high * 0.4;  
	
	cvReleaseImage( &imge );  
	cvReleaseHist(&hist);  
}  

