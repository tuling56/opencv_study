#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <cxcore.h>
//#include <iostream.h>
#include <string.h>
#include <stdio.h>

//#pragma comment(lib,"cv")
//#pragma comment(lib,"cvaux")
//#pragma comment(lib,"cxcore")
//#pragma comment(lib,"highgui")

int Histogram3DBlock[3] = {256,256,256};//3维直方图，每个维度上的bin个数
float MHistogramRange1[6]={0,255,0,255,0,255};
float *MHistogramRange[3]={&MHistogramRange1[0],&MHistogramRange1[2],&MHistogramRange1[4]};

void Print3DHistogram(CvHistogram *Histogram,int BlockSize);

int mutiDim(char*filename)
{
	CvHistogram *Histogram1;
	IplImage *Image1=cvLoadImage(filename,1);
	IplImage *RedImage=cvCreateImage(cvGetSize(Image1),8,1);
	IplImage *GreenImage=cvCreateImage(cvGetSize(Image1),8,1);
	IplImage *BlueImage=cvCreateImage(cvGetSize(Image1),8,1);
	IplImage *ImageArray[3]={RedImage,GreenImage,BlueImage};

	cvSplit(Image1,BlueImage,GreenImage,RedImage,0);
	Histogram1 = cvCreateHist(3,Histogram3DBlock,CV_HIST_SPARSE,MHistogramRange);

	cvCalcHist(ImageArray,Histogram1);

	printf("3D Historgram Data\n");
	Print3DHistogram(Histogram1,256);

	cvNamedWindow("OriginalImg",1);
	cvShowImage("OriginalImg",Image1);

	cvWaitKey(0);
	return 0;
}

void Print3DHistogram(CvHistogram *Histogram,int BlockSize)
{
	for(int i=0;i<BlockSize;i++)
	{
		for(int j=0;j<BlockSize;j++)
		{
			for(int k=0;k<BlockSize;k++)
			{
				if(cvQueryHistValue_3D(Histogram,i,j,k)>10)
				{
					printf("%5.f",cvQueryHistValue_3D(Histogram,i,j,k));
				}
			}
		}
	}
}


