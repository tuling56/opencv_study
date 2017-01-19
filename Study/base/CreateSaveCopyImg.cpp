#include"../opencv.h";

int Create_Save_Copy_Img( char* filepath)
{
	IplImage* pImg; 

	//载入图像，强制转化为Gray
	if(	(pImg = cvLoadImage(filepath , 0)) != 0 )
	{

		IplImage* pImg2 = cvCreateImage(cvGetSize(pImg),pImg->depth,pImg->nChannels);
		cvCopy(pImg, pImg2, NULL);
		cvSaveImage("fuzhi.png", pImg2);

		cvNamedWindow( "Image", 1 );
		cvShowImage( "Image", pImg );
		cvWaitKey(0); 

		cvDestroyWindow( "Image" );
		cvReleaseImage( &pImg ); 
		cvReleaseImage( &pImg2 ); 
		return 0;
	}


}