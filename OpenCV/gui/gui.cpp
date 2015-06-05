#include <opencv2/highgui/highgui_c.h>

//CvButtonCallback *callbackButton(int, void*)
//{
//
//}
//
//CvButtonCallback  * callbackButton1(int, void*)
//{
//
//}
//
//CvButtonCallback  *callbackButton2(int, void*)
//{
//
//}
//
//CvButtonCallback  *on_mouse(int, void*)
//{
//
//}


int  main(int  argc, char  *argv[])
{
	int  value = 50;
	int  value2 = 0;
	cvNamedWindow("main1", CV_WINDOW_NORMAL);
	cvNamedWindow("main2", CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	cvCreateTrackbar("track1", "main1", &value, 255, NULL);     //OK  tested
	
	char*  nameb1 = "button1";
	char*  nameb2 = "button2";
	
	cvCreateButton(nameb1, NULL, nameb1, CV_CHECKBOX, 1);
	cvCreateButton(nameb2, NULL, nameb2, CV_CHECKBOX, 0);
	cvCreateTrackbar("track2", NULL, &value2, 255, NULL);
	cvCreateButton("button5", NULL, NULL, CV_RADIOBOX, 0);
	cvCreateButton("button6", NULL, NULL, CV_RADIOBOX, 1);
	cvSetMouseCallback("main2", NULL, NULL);
	
	IplImage*  img1 = cvLoadImage("samples/fruits.jpg");
	IplImage*  img2 = cvCreateImage(cvGetSize(img1), 8, 3);
	CvCapture*  video = cvCaptureFromFile("samples/test.avi");
	IplImage*  img3 = cvCreateImage(cvGetSize(cvQueryFrame(video)), 8, 3);
	while (cvWaitKey(33) != 27)
	{
		cvAddS(img1, cvScalarAll(value), img2);
		cvAddS(cvQueryFrame(video), cvScalarAll(value2), img3);
		cvShowImage("main1", img2);
		cvShowImage("main2", img3);
	}
	
	cvDestroyAllWindows();
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&img3);
	cvReleaseCapture(&video);
	return  0;
}