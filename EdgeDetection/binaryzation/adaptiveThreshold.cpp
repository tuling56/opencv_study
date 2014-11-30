//-----------------------------------【说明】------------------------------
//	描述:比较Treshold和AdaptiveThreshold的效果区别
//  功能：cvAdaptiveThreshold()会使用Otsu算法计算得到一个全局阈值，
//		  然后根据这个阈值进行二值化
//  参考：http://blog.csdn.net/hitwengqi/article/details/6856768
//-------------------------------------------------------------------------

#include <highgui.h>
#include <math.h>
#include <cv.h>


int adaptiveThreshold(char*path)
{
	IplImage *Igray = 0, *It = 0, *Iat;

	/*if( argc != 7 )
	{
		return -1;
	}*/
	
	//输入命令行
	//double threshold = (double)atof( argv[1] ); //convert string to double
	//int threshold_type = atoi( argv[2] ) ? CV_THRESH_BINARY : CV_THRESH_BINARY_INV;
	//int adaptive_method = atoi( argv[3] ) ? CV_ADAPTIVE_THRESH_MEAN_C : CV_ADAPTIVE_THRESH_GAUSSIAN_C;
	//int block_size = atoi( argv[4] );
	//double offset = (double)atof( argv[5] );

	//这两个参数是对cvThreshold()设置的
	double threshold =  70;
	int threshold_type =CV_THRESH_BINARY;

    //这两个参数是对cvAdaptiveThreshold（）设置的
	int adaptive_method = CV_ADAPTIVE_THRESH_GAUSSIAN_C;
	int block_size = 3;
	double offset = 15.0;
	
	//加载灰度图
	if( ( Igray = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE ) ) == 0 ){
		return -1;
	}
	
	//创建同样大小8位灰度图用于输出
	It = cvCreateImage( cvSize( Igray -> width, Igray -> height ), IPL_DEPTH_8U, 1 ); //单通道8位灰度图
	Iat = cvCreateImage( cvSize( Igray -> width, Igray -> height ), IPL_DEPTH_8U, 1 );
	
	//阈值化
	cvThreshold( Igray, It, threshold, 255, threshold_type );
	cvAdaptiveThreshold( Igray, Iat, 255, adaptive_method, CV_THRESH_BINARY_INV, block_size, offset );
	
	//命名窗体输出
	cvNamedWindow( "Raw", 1 );
	cvNamedWindow( "Threshold", 1 );
	cvNamedWindow( "Adaptive Threshold", 1 );
	cvShowImage( "Raw", Igray );
	cvShowImage( "Threshold", It );
	cvShowImage( "Adaptive Threshold", Iat );
	cvWaitKey(0);
	
	//回收内存
	cvReleaseImage( &Igray );
	cvReleaseImage( &It );
	cvReleaseImage( &Iat );
	cvDestroyWindow( "Raw" );
	cvDestroyWindow( "Threshold" );
	cvDestroyWindow( "Adaptive Threshold" );

	return 0;
}