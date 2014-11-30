#include"../test.h";

int Canny(char*filename)
{
	IplImage* pImg = NULL; 
	IplImage* pCannyImg = NULL;

	//载入图像，强制转化为Gray
	if( (pImg = cvLoadImage( filename, 0)) != 0 )
	{
		
		//为canny边缘图像申请空间
		pCannyImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,	1);
		
		//canny边缘检测
		cvCanny(pImg, pCannyImg, 50, 200, 3);

		//创建窗口
		cvNamedWindow("src", 1);
		cvNamedWindow("canny",1);


		//显示图像
		/*cvShowImage( "src", pImg );
		cvShowImage( "canny", pCannyImg );*/
	
	
		 cvSaveImage("aPICT0025.jpg",pCannyImg);
	
		
		//cvWaitKey(0); //等待按键

		//销毁窗口
		cvDestroyWindow( "src" );
		cvDestroyWindow( "canny" );
		//释放图像
		cvReleaseImage( &pImg ); 
		cvReleaseImage( &pCannyImg ); 
			
		}
	  return 0;

 }



