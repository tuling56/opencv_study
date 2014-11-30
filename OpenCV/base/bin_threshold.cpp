/*
 *说明：cvAdpativeThreshold()自适应灰度二值化和cvThreshold（）的简单比较
 *   Notes:本程序中使用了cvCreateTrackbar()
 *	
 */ 

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>

using namespace std;
using namespace cv;

int mythreshold(char*filename)
{
	IplImage *src_image = 0;
	IplImage *dst_image = 0;
	IplImage *dst_image_adaptive = 0;
	int c;
	int threshold_type;
	int thresh_val;
	int max_val;
	int adaptive_method;
	int block_size;
	int offset;

	if( (src_image = cvLoadImage( filename, 0)) == 0 ){
			printf("读取%s文件失败",filename);
			return -1;
	}

	dst_image = cvCreateImage(cvSize(src_image->width,src_image->height), IPL_DEPTH_8U, 1);
	dst_image_adaptive = cvCreateImage(cvSize(src_image->width,src_image->height), IPL_DEPTH_8U, 1);
	
	cvNamedWindow( "Threshold", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "ThresholdAdaptive", CV_WINDOW_AUTOSIZE );

	//创建6个控制条
	threshold_type = CV_THRESH_BINARY;
	int threshold_typeBef = CV_THRESH_BINARY+1;
	cvCreateTrackbar( "threshold type", "Threshold", &threshold_type, 1, 0 );

	thresh_val = 100;
	int thresh_valBef = 0;
	cvCreateTrackbar( "thresh val", "Threshold", &thresh_val, 255, 0 );

	max_val = 100;
	int max_valBef = 0;
	cvCreateTrackbar( "max val", "Threshold", &max_val, 255, 0 );

	adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C;
	int adaptive_methodBef = 1;
	cvCreateTrackbar( "adaptive method", "ThresholdAdaptive", &adaptive_method, 1, 0 );

	block_size = 1;
	int block_sizeBef = 0;
	cvCreateTrackbar( "block size", "ThresholdAdaptive", &block_size, 60, 0 );

	offset = 30;
	int offsetBef = 0;
	cvCreateTrackbar( "offset", "ThresholdAdaptive", &offset, 60, 0 );


	for(;;)
	{
		if( (threshold_typeBef != threshold_type)    ||
			(thresh_valBef != thresh_val )           ||
			(max_valBef != max_val)                  ||
			(adaptive_methodBef != adaptive_method ) ||
			(block_sizeBef != block_size)            ||
			(offsetBef != offset)
		  )
		{
			cvAdaptiveThreshold(src_image, dst_image_adaptive, (double)max_val, adaptive_method, threshold_type, block_size*2+3, (double)(offset-30));
			cvShowImage("ThresholdAdaptive", dst_image_adaptive);

			if(max_val < thresh_val )
			{
				max_val = thresh_val;
			}
			cvThreshold(src_image, dst_image, (double)thresh_val, (double)max_val, threshold_type);
			cvShowImage("Threshold", dst_image);

			threshold_typeBef = threshold_type;
			thresh_valBef = thresh_val;
			max_valBef = max_val;
			adaptive_methodBef = adaptive_method;
			block_sizeBef = block_size;
			offsetBef = offset;

			//printf("The edge detection method is \n");
		}
		c = cvWaitKey(30);
		if( c == 'q' || c == 'Q' || (c & 255) == 27 || (c & 255) == 32 )
			break;
	}

	cvDestroyWindow( "Threshold" );
	cvDestroyWindow( "ThresholdAdaptive" );
	cvReleaseImage( &src_image ); 
	cvReleaseImage( &dst_image ); 
	cvReleaseImage( &dst_image_adaptive ); 
	return 0;

}