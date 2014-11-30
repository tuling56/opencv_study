#include "cv.h"  
#include "highgui.h"  
#include <math.h>  

int resize(int argc,char*argv[])  
{  
	IplImage* src = 0;  
	IplImage* dst = 0;  
	double scale=0.618;//缩放比例  
	CvSize dst_size;//定义二维化矩形框  

	if((argc==2) && (src =cvLoadImage(argv[1],1))!=0)  
	{  
		dst_size.width = (int)(src->width * scale);  
		dst_size.height = (int)(src->height * scale);//确定新图的矩形框  
		dst=cvCreateImage(dst_size,src->depth,src->nChannels);//创建图像头  
		cvResize(src,dst,CV_INTER_LINEAR);//使用双线性差值减小图像。  
		cvNamedWindow("src",CV_WINDOW_AUTOSIZE);  
		cvNamedWindow("dst",CV_WINDOW_AUTOSIZE);  
		cvShowImage("src",src);  


		cvShowImage("dst",dst);  
		cvWaitKey(-1);  
		cvReleaseImage( &src );  
		cvReleaseImage( &dst );  
		cvDestroyWindow("src");  
		cvDestroyWindow("dst");  
	}  

	return 0;  

} 