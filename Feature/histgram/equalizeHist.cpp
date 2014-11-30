//-----------------------------------【函数】------------------------------
//	描述:直方图均衡化，
//  说明:opencv的直方图均衡化函数cvEqualizeHist()针对的都是单通道，所以对于彩色图像要先进行通道分离，
//		  对单通道图像分别进行均衡化，最后再合并。		
//  参考：http://blog.csdn.net/sundy_2004/article/details/7259614
//-------------------------------------------------------------------------

#include <cv.h>
#include <highgui.h>

int equalizeHist(char*path)
{
	int i;
	IplImage* src = cvLoadImage( path, 1 );
	IplImage* imgChannel[4] = { 0, 0, 0, 0 };
	IplImage* dst = cvCreateImage( cvGetSize( src ), IPL_DEPTH_8U, 3 );

	if( src )
	{
		for( i = 0; i < src -> nChannels; i++ )
		{
			imgChannel[i] = cvCreateImage( cvGetSize( src ), IPL_DEPTH_8U, 1 );  //要求单通道图像才能直方图均衡化
		}
		
		//通道分离
		cvSplit( src, imgChannel[0], imgChannel[1], imgChannel[2], imgChannel[3] );
		for( i = 0; i < dst -> nChannels; i++ )
		{
			//直方图均衡化，原始图像和目标图像必须是单通道
			cvEqualizeHist( imgChannel[i], imgChannel[i] );
		}

		//通道组合
		cvMerge( imgChannel[0], imgChannel[1], imgChannel[2], imgChannel[3], dst );
		cvNamedWindow( "src", 1 );
		cvShowImage( "src", src );
		cvNamedWindow( "Equalize", 1 );
		cvShowImage( "Equalize", dst );

		cvWaitKey(0);
		//释放资源
		for( i = 0; i < src -> nChannels; i++ )
		{
			if( imgChannel[i] )
			{
				cvReleaseImage( &imgChannel[i] );
				//imgChannel[i] = 0;
			}
		}
		cvReleaseImage( &dst );
	}

	return 0;
}