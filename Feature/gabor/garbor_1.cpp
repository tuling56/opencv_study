//首先包含头文件
#include "cvgabor.h"

int garbor()
{
	//创建一个方向是PI/4而尺度是3的gabor
	double Sigma = 2*PI;
	double F = sqrt(2.0);
	CvGabor *gabor1 = new CvGabor; gabor1->Init(PI/4, 3, Sigma, F);

	//获得实部并显示它
	IplImage *kernel = cvCreateImage( cvSize(gabor1->get_mask_width(), gabor1->get_mask_width()), IPL_DEPTH_8U, 1);
	kernel = gabor1->get_image(CV_GABOR_REAL);
	cvNamedWindow("Gabor Kernel", 1);
	cvShowImage("Gabor Kernel", kernel);
	cvWaitKey(0);

	//载入一个图像并显示
	IplImage *img = cvLoadImage( "Crop1.bmp", CV_LOAD_IMAGE_GRAYSCALE );
	cvNamedWindow("Original Image", 1);
	cvShowImage("Original Image", img);
	cvWaitKey(0);  


	 
		   //获取载入图像的gabor滤波响应的实部并且显示

	IplImage *reimg = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	gabor1->conv_img(img, reimg, CV_GABOR_REAL);
	cvNamedWindow("Real Response", 1);
	cvShowImage("Real Response",reimg);
	cvWaitKey(0);
	cvDestroyWindow("Real Response");


	//获取载入图像的gabor滤波响应的虚部并且显示
	IplImage *reimg = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	gabor1->conv_img(img, reimg, CV_GABOR_IMAG);
	cvNamedWindow("Imaginary Response", 1);
	cvShowImage("Imaginary Response",reimg);
	cvWaitKey(0);
	cvDestroyWindow("Imaginary Response");


														
	//获取载入图像的gabor滤波响应的模并且显示
	IplImage *reimg = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	gabor1->conv_img(img, reimg, CV_GABOR_MAG);
	cvNamedWindow("Magnitude Response", 1);
	cvShowImage("Magnitude Response",reimg);
	cvWaitKey(0);
	cvDestroyWindow("Magnitude Response");
}