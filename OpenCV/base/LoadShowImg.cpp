#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std; 

//OpenCV1.0接口的
void Load_Show_Img(char* filepath);

int Read_Show_main( int argc, char** argv )
{ 

	Mat image;
	image = imread("samples/1.png", CV_LOAD_IMAGE_COLOR);	// Read the file

	if(! image.data )                              // Check for invalid input
	{
		cout <<  "Could not open or find the image" << endl ;
		return -1;
	}

	//插入文字  
	//参数为：承载的图片，插入的文字，文字的位置（文本框左下角），字体，大小，颜色  
	string words= "good luck";  
	putText( image, words, Point( image.rows/2,image.cols/4),CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0) );  


	namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
	imshow( "Display window", image );                   // Show our image inside it.

	waitKey(0);											 // Wait for a keystroke in the window
	return 0;
}


void Load_Show_Img(char* filepath)
{

	/* cvLoadImage( filename, -1 ); 默认读取图像的原通道数 
		cvLoadImage( filename, 0 );强制转化读取图像为灰度图 
		cvLoadImage( filename, 1 );读取彩色图 
		*/

    IplImage *pImg;
	if(	(pImg = cvLoadImage( filepath, 1)) != 0 )
	{
		cvNamedWindow( "Image", 1 );//创建窗口
		cvShowImage( "Image", pImg );//显示图像
		cvWaitKey(0); //等待按键
		cvDestroyWindow( "Image" );//销毁窗口
		cvReleaseImage( &pImg ); //释放图像
		cvWaitKey(0); //等待按键

	}

	else{
		cout<<"加载失败"<<endl;
		cvWaitKey(0); //等待按键

	}

}


