//Function：矩形检测
//Source:
//Status:
//Improve:
//Info:[11/15/2014 jmy]

#ifdef _CH_
#pragma package <opencv>
#endif

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

int thresh = 50;
IplImage* img = 0;
IplImage* img0 = 0;
CvMemStorage* storage = 0;
const char* wndname = "Square Detection Demo";
char* names[] = { "samples/square/rect.png","samples/square/4.bmp",0 }; //要加载的图片


// helper function:求两个向量的夹角
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2 
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{
	double dx1 = pt1->x - pt0->x;
	double dy1 = pt1->y - pt0->y;
	double dx2 = pt2->x - pt0->x;
	double dy2 = pt2->y - pt0->y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
CvSeq* findSquares4( IplImage* img, CvMemStorage* storage )
{
	CvSeq* contours; //轮廓序列
	int i, c, l, N = 11;//N为什么设置这么低
	CvSize sz = cvSize( img->width & -2, img->height & -2 );//宽和高变为原来的1/2
	IplImage* timg = cvCloneImage( img ); 
	IplImage* gray = cvCreateImage( sz, 8, 1 ); 
	
	//进行金字塔分解的原因？
	IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
	IplImage* tgray;
	CvSeq* result; //结果序列
	double s, t;
	
	// create empty sequence that will contain points - 4 points per square (the square's vertices)
	CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage ); //序列中存的是点，即点序列，序列存储在storage中

	// select the maximum ROI in the image with the width and height divisible by 2
	cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));

	// down-scale and upscale the image to filter out the noise
	cvPyrDown( timg, pyr, 7 );
	cvPyrUp( pyr, timg, 7 ); //经过分解和重构后，又变成原来的大小了，但噪声被过滤了
	tgray = cvCreateImage( sz, 8, 1 );

	// find squares in every color plane of the image
	for( c = 0; c < 3; c++ )
	{
		// extract the c-th color plane
		cvSetImageCOI( timg, c+1 );
		cvCopy( timg, tgray, 0 );

		// try several threshold levels
		for( l = 0; l < N; l++ )
		{
			// hack: use Canny instead of zero threshold level.
			// Canny helps to catch squares with gradient shading   
			if( l == 0 )
			{
				// apply Canny. Take the upper threshold from slider
				// and set the lower to 0 (which forces edges merging) 
				cvCanny( tgray, gray, 0, thresh, 5 ); 
				// dilate canny output to remove potential holes between edge segments 
				cvDilate( gray, gray, 0, 1 );
			}
			else
			{
				// apply threshold if l!=0:
				//tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
				cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
			}

			// find contours and store them all as a list
			//storage是临时存储区，contours是存储轮廓点的CvSeq实例
			cvFindContours( gray, storage, &contours, sizeof(CvContour),
				            CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

			// 轮廓遍历
			while( contours )
			{
				// approximate contour with accuracy proportional to the contour perimeter
				// 轮廓的多边形逼近,这里指的是一个轮廓的多边形逼近，轮廓的多边形逼近可能不是矩形，下面的result->total即判定
				// 序列中元素的数目，其实序列指针可以理解为二维数组
				result = cvApproxPoly( contours, sizeof(CvContour), storage,
					                   CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
				// square contours should have 4 vertices after approximation
				// relatively large area (to filter out noisy contours) and be convex.
				// Note: absolute value of an area is used because area may be positive or negative 
				//- in accordance with the contour orientation
				if( result->total == 4 &&cvCheckContourConvexity(result)&&
					fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 500  )
				{
					s = 0;
					for( i = 0; i < 5; i++ )
					{
						// find minimum angle between joint edges (maximum of cosine)
						if( i >= 2 )
						{
							t = fabs(angle(
								(CvPoint*)cvGetSeqElem( result, i ),
								(CvPoint*)cvGetSeqElem( result, i-2 ),
								(CvPoint*)cvGetSeqElem( result, i-1 )));
							s = s > t ? s : t;
						}
					}

					// if cosines of all angles are small (all angles are ~90 degree) then write quandrange
					// vertices to resultant sequence 
					if( s < 0.5 )
						for( i = 0; i < 4; i++ )
							cvSeqPush( squares,(CvPoint*)cvGetSeqElem(result, i ));
				}

				// take the next contour,水平方向上的下一个轮廓序列
				contours = contours->h_next;
			}
		}
	}

	// release all the temporary images
	cvReleaseImage( &gray );
	cvReleaseImage( &pyr );
	cvReleaseImage( &tgray );
	cvReleaseImage( &timg );

	return squares;
}


// the function draws all the squares in the image
void drawSquares( IplImage* img, CvSeq* squares )
{
	CvSeqReader reader;
	IplImage* cpy = cvCloneImage( img );

	// initialize reader of the sequence
	cvStartReadSeq( squares, &reader, 0 );

	// read 4 sequence elements at a time (all vertices of a square)
	for(int i = 0; i < squares->total; i += 4 )
	{
		CvPoint pt[4], *rect = pt;
		int count = 4;

		// read 4 vertices
		CV_READ_SEQ_ELEM( pt[0], reader );
		CV_READ_SEQ_ELEM( pt[1], reader );
		CV_READ_SEQ_ELEM( pt[2], reader );
		CV_READ_SEQ_ELEM( pt[3], reader );

		// draw the square as a closed polyline 
		cvPolyLine( cpy, &rect, &count, 1, 1, CV_RGB(255,0,0), 0.5, CV_AA, 0 );
	}

	// show the result image
	cvShowImage( wndname, cpy );
	cvReleaseImage( &cpy );
}



//矩形检测主入口点
int square_detection()
{
	int  c;
	// create memory storage that will contain all the dynamic data
	storage = cvCreateMemStorage(0);

	for( int i = 0; names[i] != 0; i++ )
	{
		img0 = cvLoadImage( names[i], 1 );
		if( !img0 )
		{
			printf("Couldn't load %s\n", names[i] );
			continue;
		}
		img = cvCloneImage( img0 );

		// create window and a trackbar (slider) with parent "image" and set callback
		// (the slider regulates upper threshold, passed to Canny edge detector) 
		cvNamedWindow( wndname, 1 );

		// find and draw the squares
		drawSquares( img, findSquares4( img, storage ) );

		// wait for key. Also the function cvWaitKey takes care of event processing
		c = cvWaitKey(0);
		
		cvReleaseImage( &img );
		cvReleaseImage( &img0 );
		
		// clear memory storage - reset free space position
		cvClearMemStorage( storage );
		if( (char)c == 27 )
			break;
	}

	cvDestroyWindow( wndname );

	return 0;
}
