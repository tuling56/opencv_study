#include <cv.h>  
#include <highgui.h>  

int images2video()  
{  
	
	char filename[50];

	double fps=22;
	CvSize size=cvSize(640,480);
	char*videoout="images2video.wmv";

	CvVideoWriter *writer=cvCreateVideoWriter(videoout,CV_FOURCC('M','J','P','G'),fps,size);  

	cvNamedWindow("转换后的视频", CV_WINDOW_AUTOSIZE);
	IplImage *bgr_frame=cvCreateImage(size,IPL_DEPTH_8U,3); 

	//读取文件夹内的指定格式的所有文件
	for (int i=1;i<24;i++)
	{
		sprintf(filename,"PicVideo/frame2video/%d.png",i);
		printf("正在处理%s\n",filename);
		if((bgr_frame=cvLoadImage(filename))!=NULL)  
		{  
			cvLine(bgr_frame,cvPoint(12,14),cvPoint(60,60),CV_RGB(30,20,100));
			cvRectangle(bgr_frame,cvPoint(12,14),cvPoint(60,60),CV_RGB(255,0,0));

			cvWriteFrame(writer,bgr_frame); //用cvWriteFrame函数将极指数空间的图像保存到writer中 
			cvShowImage("转换后的视频", bgr_frame); //在窗口中显示生成的视频；  

			char c=cvWaitKey(120);  
			if (c == 27) break; //当输入ESC键时，退出窗口；  
		}  
		else
		{
			printf("加载图片失败\n");
			return -1;
		}
	
    }

	cvReleaseVideoWriter(&writer);  
	cvReleaseImage(&bgr_frame);   
	
	return 0;  
}  