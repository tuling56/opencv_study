/*
 *	把用标准hough变换检测出的最强直线转换成ax+by+c=0形
 */

#include <cv.h>
#include <highgui.h>
#include <math.h>

int stdhoughlinedete(char *filename)
{
	IplImage* imgTest = cvLoadImage(filename,0);
	if (!imgTest)
	{
		printf("加载图片%s失败",filename);
		exit(0);
	}
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;

	lines = cvHoughLines2( imgTest, storage, CV_HOUGH_STANDARD,1,CV_PI/180, 5, 0, 0 );
	float* line = (float*)cvGetSeqElem(lines,0);
	float rho = line[0];          // distance from (0,0) to the line
	float theta = line[1];        // angle between x-axis and the normal to the line

	double thetaDegree = theta/3.1415*180;

	IplImage* imgRes = cvCreateImage(cvSize(imgTest->width,imgTest->height),IPL_DEPTH_8U,3);
	cvSetZero(imgRes);

	double a,b,c;                // ax+by+c=0
	if (theta>CV_PI/2)
	{
		theta = CV_PI-theta;
		if (theta>CV_PI/4)
		{
			a = 1/tan(theta);
			b = -1;
			c = rho/sin(theta);
		}
		else
		{
			a = 1;
			b = -tan(theta);
			c = rho/cos(theta);
		}
	}
	else
	{
		if (theta>CV_PI/4)
		{
			a = -1/tan(theta);
			b = -1;
			c = rho/sin(theta);
		}
		else
		{
			a = -1;
			b = -tan(theta);
			c = rho/cos(theta);
		}
	}

	if (abs(a)>abs(b))        // using x = -b/a*y-c/a
	{
		for (int h=0;h<imgRes->height;h++)
		{
			int w = -b/a*h-c/a;
			if (w<imgRes->width && w>=0)
			{
				imgRes->imageData[h*imgRes->widthStep+3*w] = 255;
			}
		}
	}
	else                    // using y = -a/b*x-c/b
	{
		for (int w=0;w<imgRes->width;w++)
		{    

			int h = -a/b*w-c/b;
			if (h<imgRes->height && h>=0)
				imgRes->imageData[h*imgRes->widthStep+3*w] = 255;
		}
	}
	cvSaveImage("linedeted.bmp",imgRes);
	cvReleaseImage(&imgRes);
	cvReleaseImage(&imgTest);

	return 0;
}