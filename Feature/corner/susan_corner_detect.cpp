/*标题：OpenCV中的Susan角点检测，当修改cvGoodFeaturesToTrack（）的use_harries参数时变成harries角点检测
 *	http://blog.csdn.net/augusdi/article/details/9012555
 */

#include <stdio.h>  
#include <cv.h>  
#include <highgui.h>  

#define max_corners 1000  

int susan_corner_detect(char*filename)  
{  
	int cornerCount=max_corners;  
	CvPoint2D32f corners[max_corners];  
	double qualityLevel;  
	double minDistance;  
	IplImage *srcImage = 0, *grayImage = 0, *corners1 = 0, *corners2 = 0;  
	int i;  
	CvScalar color = CV_RGB(255,0,0);  
	//char* filename = argc == 2 ? argv[1] : (char*)"lena.jpg"; 
	//cvNamedWindow( "input", 1 ); // create HighGUI window with name "image"  

	//Load the image to be processed  
	srcImage = cvLoadImage(filename,1);  
	grayImage = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);  

	//copy the source image to copy image after converting the format  
	cvCvtColor(srcImage, grayImage, CV_BGR2GRAY);  

	//create empty images of same size as the copied images  
	corners1= cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_32F, 1);  
	corners2= cvCreateImage(cvGetSize(srcImage),IPL_DEPTH_32F, 1);  

	cvGoodFeaturesToTrack (grayImage, corners1, corners2, corners, &cornerCount, 0.05, 5, 0);  
	printf("num corners found: %d/n", cornerCount);  

	// draw circles at each corner location in the gray image and print out a list the corners   
	if(cornerCount>0)   
	{  
		for (i=0; i<cornerCount;i++)       
		{  
			cvCircle(srcImage, cvPoint((int)(corners[i].x), (int)(corners[i].y)), 6, color, 1, CV_AA, 0);  
		}  
	}  

	//cvShowImage( "input", srcImage );  


	//图像缩放
	cvNamedWindow("scale_dst",1);
	CvSize dst_size;
	double scale=0.5;
	IplImage*scale_dst=0;
	dst_size.width = (int)(srcImage->width * scale);  
	dst_size.height = (int)(srcImage->height * scale);//确定新图的矩形框  
	scale_dst=cvCreateImage(dst_size,srcImage->depth,srcImage->nChannels);//创建图像头  
	cvResize(srcImage,scale_dst,CV_INTER_LINEAR);//使用双线性差值减小图像。 
	cvShowImage("scale_dst",scale_dst);
	cvSaveImage("samples//scale_dst.JPG",scale_dst);

	cvWaitKey(0);

	cvDestroyAllWindows();

	cvReleaseImage(&srcImage);  
	cvReleaseImage(&grayImage);  
	cvReleaseImage(&corners1);  
	cvReleaseImage(&corners2);  

	cvReleaseImage(&scale_dst);

	cvWaitKey(0);  
	return 0;  
}  