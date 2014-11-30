#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>  

int inter_value(char*filename)
{
	IplImage*src=cvLoadImage(filename);
	IplImage*dst=0;
	CvSize dst_cvsize;
	if (!src)
	{
	  printf("failure\n");
	  getchar();
	}
	else
	{ 
       double scale=4;
		dst_cvsize.width=(int)(src->width*scale);
		dst_cvsize.height=(int)(src->height*scale);
		dst=cvCreateImage(dst_cvsize,src->depth,src->nChannels);


		cvResize(src,dst,CV_INTER_CUBIC);
		//   CV_INTER_NN - 最近邻插值,
		//   CV_INTER_LINEAR - 双线性插值 (缺省使用)
		//   CV_INTER_AREA - 使用象素关系重采样。当图像缩小时候，该方法可以避免波纹出现。当图像放大时，类似于 CV_INTER_NN 方法
		//   CV_INTER_CUBIC - 立方插值.


		cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("dst",CV_WINDOW_AUTOSIZE);
		cvShowImage("src",src);
		cvShowImage("dst",dst);

		cvWaitKey();

		cvReleaseImage(&src);
		cvReleaseImage(&dst);
		cvDestroyWindow("scr");
		cvDestroyWindow("dst");

	 
	}
	
	return 0;

}