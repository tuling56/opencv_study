#include <cv.h>  
#include <highgui.h>  

int read_write_video(char*videoin,char*videoout)  
{  

	CvCapture *capture=NULL;  
	capture=cvCreateFileCapture(videoin);  
	if(!capture) return -1;  
	IplImage *bgr_frame=cvQueryFrame(capture);  
	double fps=cvGetCaptureProperty(capture,CV_CAP_PROP_FPS);  
	//用cvSize函数建立一个CvSize类型的变量size，其宽度和高度与输入视频文件相同  
	CvSize size=cvSize((int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH),  
		(int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT));  
	//用cvCreateVideoWriter函数新建一个CvVideoWriter*类型变量，用来输出视频文件  
	CvVideoWriter *writer=cvCreateVideoWriter(videoout,CV_FOURCC('M','J','P','G'),fps,size);  

	cvNamedWindow("转换后的视频", CV_WINDOW_AUTOSIZE);
	IplImage *logpolar_frame=cvCreateImage(size,IPL_DEPTH_8U,3); 
	while((bgr_frame=cvQueryFrame(capture))!=NULL)  
	{  
		//用cvLogPolar函数把图像映射到极指数空间  
		/*cvLogPolar(bgr_frame,logpolar_frame,cvPoint2D32f(bgr_frame->width/2,  
		bgr_frame->height/2),40,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);  */
		
		cvLine(bgr_frame,cvPoint(12,14),cvPoint(60,60),CV_RGB(30,20,100));
		cvRectangle(bgr_frame,cvPoint(12,14),cvPoint(60,60),CV_RGB(255,0,0));
		  
		cvWriteFrame(writer,bgr_frame); //用cvWriteFrame函数将极指数空间的图像保存到writer中 
		cvShowImage("转换后的视频", bgr_frame); //在窗口中显示生成的视频；  

		char c=cvWaitKey(33);  
		if (c == 27) break; //当输入ESC键时，退出窗口；  


	}  
	
	cvReleaseVideoWriter(&writer);  
	cvReleaseImage(&logpolar_frame);  
	cvReleaseCapture(&capture);  
	
	return 0;  
}  