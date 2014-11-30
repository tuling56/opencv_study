#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>

int Contour_detection( char*filename)
{
	//声明IplImage指针
	IplImage* pImg = NULL; 
	IplImage* pContourImg = NULL;


	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contour = 0;
	int mode = CV_RETR_EXTERNAL;

	/*if( argc == 3)
		if(strcmp(argv[2], "all") == 0)*/
			mode = CV_RETR_CCOMP; //内外轮廓都检测


	//创建窗口
	cvNamedWindow("src", 1);
	cvNamedWindow("contour",1);


	//载入图像，强制转化为Gray
	if((pImg = cvLoadImage(filename, 0)) != 0 )
	{

		cvShowImage( "src", pImg );

		//为轮廓显示图像申请空间
		//3通道图像，以便用彩色显示
		pContourImg = cvCreateImage(cvGetSize(pImg),
			IPL_DEPTH_8U,
			3);
		//copy source image and convert it to BGR image
		cvCvtColor(pImg, pContourImg, CV_GRAY2BGR);


		//查找contour
		cvFindContours( pImg, storage, &contour, sizeof(CvContour), 
			mode, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	}
	else
	{
		//销毁窗口
		cvDestroyWindow( "src" );
		cvDestroyWindow( "contour" );
		cvReleaseMemStorage(&storage);

		return -1;
	}




	//将轮廓画出    
	cvDrawContours(pContourImg, contour, 
		CV_RGB(0,0,255), CV_RGB(255, 0, 0), 
		2, 2, 8, cvPoint(0,0));
	//显示图像
	cvShowImage( "contour", pContourImg );

	cvWaitKey(0);


	//销毁窗口
	cvDestroyWindow( "src" );
	cvDestroyWindow( "contour" );
	//释放图像
	cvReleaseImage( &pImg ); 
	cvReleaseImage( &pContourImg ); 

	cvReleaseMemStorage(&storage);

	return 0;
}