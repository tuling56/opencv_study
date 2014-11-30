#include "cvgabor.h"   

int garbor(char*filename)
{  
  
    //创建一个方向是PI/4而尺度是3的gabor   
    double Sigma = 2*PI;    
    double F = sqrt(2.0);    
    CvGabor *gabor1 = new CvGabor;    
    gabor1->Init(PI/4, 3, Sigma, F);  
      
    //获得实部并显示它   
    IplImage *kernel = cvCreateImage( cvSize(gabor1->get_mask_width(), gabor1->get_mask_width()), IPL_DEPTH_8U, 1);  
    kernel = gabor1->get_image(CV_GABOR_REAL);  
    cvNamedWindow("Gabor Kernel", 1);  
    cvShowImage("Gabor Kernel", kernel);  
    cvWaitKey(0);  
  
    //载入一个图像并显示   
    IplImage *img = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE );  
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
//  IplImage *reimg = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);   
    gabor1->conv_img(img, reimg, CV_GABOR_IMAG);  
    cvNamedWindow("Imaginary Response", 1);  
    cvShowImage("Imaginary Response",reimg);  
    cvWaitKey(0);  
    cvDestroyWindow("Imaginary Response");  
  
    //获取载入图像的gabor滤波响应的模并且显示   
//  IplImage *reimg = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);   
    gabor1->conv_img(img, reimg, CV_GABOR_MAG);  
    cvNamedWindow("Magnitude Response", 1);  
    cvShowImage("Magnitude Response",reimg);  
    cvWaitKey(0);  
    cvDestroyWindow("Magnitude Response");  
  
    /* 
    //这个响应可以被取样为8位的灰度图。如果你要原始的浮点类型的数据，你可以这样做 
    IplImage *reimg = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_32F, 1); 
    gabor1->conv_img(img, reimg, CV_GABOR_MAG); 
    //然而，这些浮点数据是不能够以上面灰度图的形式简单的显示，但是它可以被保存在一个XML文件中。 
    cvSave( "reimg.xml", (IplImage*)reimg, NULL, NULL, cvAttrList(0,0)); 
    */  
	return 0;
}  