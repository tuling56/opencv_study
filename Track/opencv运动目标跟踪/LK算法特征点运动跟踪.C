#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

const int MAX_CORNERS = 500;
int main(int argc, char** argv) {
// Initialize, load two images from the file system, and
// allocate the images and other structures we will need for
// results.
//
IplImage* imgA = cvLoadImage("OpticalFlow0.jpg",CV_LOAD_IMAGE_GRAYSCALE);
IplImage* imgB = cvLoadImage("OpticalFlow1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
CvSize img_sz = cvGetSize( imgA );
int win_size = 10;
IplImage* imgC = cvLoadImage("OpticalFlow1.jpg",CV_LOAD_IMAGE_UNCHANGED);

// The first thing we need to do is get the features
// we want to track.
//
IplImage* eig_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );
IplImage* tmp_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );
int corner_count = MAX_CORNERS;
CvPoint2D32f* cornersA = new CvPoint2D32f[ MAX_CORNERS ];
cvGoodFeaturesToTrack(
imgA,//the input image
eig_image,//temp image whose result is meaningful
tmp_image,//temp image
cornersA,//contains the result points
&corner_count,//the maximum number of points
0.01,//indicates the minimal acceptable lower eigenvalue for a point to be included as a corner
5.0,//guarantees that no two returned points are within the indicated number of pixels.
0,//no mask is used
3,// the region around a given pixel that is considered when computing the autocorrelation matrix of derivatives.
0,//use the the Shi-Tomasi deinition
0.04
);
/* Further find more accurate points */
cvFindCornerSubPix(
imgA,
cornersA,
corner_count,
cvSize(win_size,win_size),
cvSize(-1,-1),
cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03)
);
// Call the Lucas Kanade algorithm
//
char features_found[ MAX_CORNERS ];
float feature_errors[ MAX_CORNERS ];
CvSize pyr_sz = cvSize( imgA->width+8, imgB->height/3 );
IplImage* pyrA = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1 );
IplImage* pyrB = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1 );
CvPoint2D32f* cornersB = new CvPoint2D32f[ MAX_CORNERS ];
cvCalcOpticalFlowPyrLK(
imgA,
imgB,
pyrA,
pyrB,
cornersA,
cornersB,
corner_count,
cvSize( win_size,win_size ),
5,
features_found,
feature_errors,
cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 ),
0
);
// Now make some image of what we are looking at:
//
for( int i=0; i<corner_count; i++ ) {
if( features_found[i]==0|| feature_errors[i]>550 ) {
// printf("Error is %f/n",feature_errors[i]);
continue;
}
// printf("Got it/n");
CvPoint p0 = cvPoint(
cvRound( cornersA[i].x ),
cvRound( cornersA[i].y )
);
CvPoint p1 = cvPoint(
cvRound( cornersB[i].x ),
cvRound( cornersB[i].y )
);
cvLine( imgC, p0, p1, CV_RGB(255,0,0),2 );
}
cvNamedWindow("ImageA",0);
cvNamedWindow("ImageB",0);
cvNamedWindow("LKpyr_OpticalFlow",0);
cvSaveImage("result_LK.jpg",imgC);
cvShowImage("ImageA",imgA);
cvShowImage("ImageB",imgB);
cvShowImage("LKpyr_OpticalFlow",imgC);
cvWaitKey(0);
return 0;
}


