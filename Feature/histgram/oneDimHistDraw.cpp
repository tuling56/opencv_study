/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	oneDimHistDraw.cpp
* Brief: 一维直方图绘制
* Source:
* Status: 
* Date:	[3/12/2015 jmy]
************************************************************************/

#include <cv.h>
#include <highgui.h>
#include <stdio.h>

int HistogramBlock = 50 ;
int HistogramBlockWidth;
float HistogramRange1[ 2 ]={ 30 , 200 };//浮点数对
float *HistogramRange[ 1 ]={ & HistogramRange1[ 0 ]};//浮点数对构成的数组

int oneDim(char *filename)
{
	IplImage *Image1;
	CvHistogram *Histogram1;
	IplImage *HistogramImage1;
	CvPoint Point1;
	CvPoint Point2;

	Image1=cvLoadImage( filename , 0 );

	Histogram1 = cvCreateHist( 1 , & HistogramBlock,CV_HIST_ARRAY,HistogramRange);
	HistogramImage1 = cvCreateImage(cvSize( 256 , 300 ), 8 , 3 );
	cvSetZero(HistogramImage1);
	HistogramImage1->origin= 1 ;
	HistogramBlockWidth=256/HistogramBlock;
	printf("The Block Width is:%d\n",HistogramBlockWidth);

	//

	cvCalcHist( & Image1,Histogram1);

	printf( "type is : %d\n" ,Histogram1->type);
	printf( "Low Bound is : %.f\n" ,Histogram1->thresh[ 0 ][ 0 ]);
	printf( "Up Bound is : %.f\n" ,Histogram1->thresh[ 0 ][ 1 ]);
	printf( "The Block is : %d\n" ,((CvMatND *) Histogram1->bins)->dim[ 0 ].size);

	printf( "\nGray Level Values:\n" );
	for ( int i= 0 ;i<HistogramBlock;i++)
	{
	  printf( "%.f \t" ,((CvMatND *) Histogram1->bins)->data.fl[i]);
	  //cvLine(HistogramImage1,cvPoint(i, 0 ),cvPoint(i,( int )(cvQueryHistValue_1D(Histogram1,i)/10 )),CV_RGB( 127 , 127 , 127 ));
	  Point1=cvPoint(i*HistogramBlockWidth,0);
	  Point2=cvPoint((i+1)*HistogramBlockWidth,(int)cvQueryHistValue_1D(Histogram1,i)/30);
	  cvRectangle(HistogramImage1,Point1,Point2,CV_RGB(220,0,0));
	}

	cvNamedWindow( "Histogram" , 1 );
	cvNamedWindow( "OriginalImg(Gray)" , 1 );
	cvShowImage( "OriginalImg(Gray)" ,Image1);
	cvShowImage( "Histogram" ,HistogramImage1);
	cvWaitKey( 0 );
	
	return 0;
} 
