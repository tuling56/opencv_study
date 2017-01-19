#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std; 

//OpenCV1.0接口的
void Load_Show_Img(char* filepath);


/*
 *	功能：OpenCV2.0接口读取和显示图像
 *  输入：
 *  输出：
 *  状态：
 */
int cv2readImg()
{ 
	Mat image;
	image = imread("samples/lena.jpg", CV_LOAD_IMAGE_COLOR);	
	if(!image.data ) {
		cout <<  "Could not open or find the image" << endl ;
		return -1;
	}

	//插入文字  
	//参数为：承载的图片，插入的文字，文字的位置（文本框左下角），字体，大小，颜色  
	string words= "good luck";  
	putText( image, words, Point( image.rows/2,image.cols/4),CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0) );  

	namedWindow( "Display window", CV_WINDOW_AUTOSIZE ); // Create a window for display.
	imshow( "Display window", image );                   // Show our image inside it.

	waitKey(0);											 // Wait for a keystroke in the window
	return 0;
}

/*
 *	功能:OpenCV1.0接口读取和显示图像
 *  输入：
 *  输出：
 *  状态：
 */

void cv1readImg()
{
	//cvLoadImage( filename, -1 ); 默认读取图像的原通道数 
	//cvLoadImage( filename, 0 );  强制转化读取图像为灰度图 
	//cvLoadImage( filename, 1 );  读取彩色图 
    IplImage *pImg;
	if(	(pImg = cvLoadImage("samples/lena.jpg", 1)) != 0 )
	{
		cvNamedWindow( "Image", 1 );
		cvShowImage( "Image", pImg );//显示图像
		cvWaitKey(0); 
		cvDestroyWindow( "Image" );
		cvReleaseImage( &pImg ); //释放图像
		cvWaitKey(0); 
	}
	else{
		cout<<"加载失败"<<endl;
		cvWaitKey(0); 
	}
}


//功能测试区
int main_read()
{
	cv2readImg();
	waitKey(0);
	return 0;
}

