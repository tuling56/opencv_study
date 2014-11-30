//-----------------------------------【HoughCircle检测】------------------------------
//	描述:在灰度图像中找圆
//  参考：http://blog.csdn.net/hitwengqi/article/details/6883299
//-------------------------------------------------------------------------

#include <highgui.h>
#include <math.h>
#include <cv.h>

int hough_circle(char*filename)
{
	IplImage* src = cvLoadImage( filename, 1);
	IplImage*gray=cvCreateImage(cvGetSize(src),src->depth,1);
	cvCvtColor(src,gray,CV_BGR2GRAY);
	IplImage* dst = cvCreateImage( cvGetSize(src),src->depth,1 );
	CvMemStorage* storage = cvCreateMemStorage(0);
	cvSmooth(gray, dst, CV_GAUSSIAN, 5, 5 );  //降噪
	
	//cvHoughCircles函数需要估计每一个像素梯度的方向，因此会在内部自动调用cvSobel,而二值边缘图像的处理是比较难的
	CvSeq* results = cvHoughCircles(  
		dst,
		storage,
		CV_HOUGH_GRADIENT,
		2,  //累加器图像的分辨率
		src->width/10
		);

	for( int i = 0; i < results->total; i++ )
	{
		float* p = ( float* )cvGetSeqElem( results, i );
		//霍夫圆变换
		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
		cvCircle(
			src,
			pt,  //确定圆心
			cvRound( p[2] ),  //确定半径
			CV_RGB(255,0, 0 )
			);  //画圆函数
	}

	cvNamedWindow( "cvHoughCircles", 1 );
	cvShowImage( "cvHoughCircles", src );
	
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&gray);
	cvReleaseImage(&dst);
	return 0;
}