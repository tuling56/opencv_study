/**
 * @function moments_demo.cpp
 * @brief Demo code to calculate moments
 *  计算不规则形状的矩，找出重心。samples\cpp\tutorial_code\ShapeDescriptors
 * @author OpenCV team
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int m_thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/**
 * @function main
 */
int moments_demo( int, char** argv )
{
  /// Load source image and convert it to gray
  src = imread("samples//200.png", 1 );

  /// Convert image to gray and blur it
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  const char* source_window = "Source";
  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src );

  createTrackbar( " Canny m_thresh:", "Source", &m_thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
}

/**
 * @function thresh_callback
 */
void thresh_callback(int, void* )
{
  Mat canny_output,src_canny;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, m_thresh, m_thresh*2, 3 );
  src.copyTo(src_canny, canny_output);
  imshow("src_canny", src_canny);


  imshow("Canny", canny_output);
  /// Find contours,是在canny图像上进行的轮廓检测
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Get the moments
  vector<Moments> mu(contours.size() );
  for( size_t i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> mc( contours.size() );
  for( size_t i = 0; i < contours.size(); i++ )
     { mc[i] = Point2f( static_cast<float>(mu[i].m10/mu[i].m00) , static_cast<float>(mu[i].m01/mu[i].m00) ); }

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
     }

  /// Show in a window
  namedWindow( "Contours", WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );

  /// Calculate the area with the moments 00 and compare with the result of the OpenCV function
  printf("\t Info: Area and Contour Length \n");
  //for( size_t i = 0; i< contours.size(); i++ )
  //   {
  //     printf(" Contour[%d] ： Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", (int)i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
  //     
	 //  Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
  //     drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
  //     circle( drawing, mc[i], 4, color, -1, 8, 0 );
  //   }
}
