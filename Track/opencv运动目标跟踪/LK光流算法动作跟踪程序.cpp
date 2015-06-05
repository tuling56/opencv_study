//--------------------------------------------------------------------- 
//  
// File        : main.cpp 
// Project     : klt tracker using CV library 
// Date        : Nov 2007  
// Author      : tulip toppo + abhishek rawat 
// Description : KLT tracker (we have used open CV functions) 
//--------------------------------------------------------------------- 
 
#ifdef _CH_ 
#pragma package <opencv> 
#endif 
 
#ifndef _EiC 
#include <cv.h> 
#include <highgui.h> 
#include <cxcore.h> 
#include <stdio.h> 
#include <ctype.h> 
#endif 
 
IplImage *image = 0, *grey = 0, *prev_grey = 0, *pyramid = 0, *prev_pyramid = 0, *swap_temp; 
 
int win_size = 10; 
const int MAX_COUNT = 500; 
CvPoint2D32f* points[2] = {0,0}, *swap_points; 
char* status = 0; 
int count = 0; 
int need_to_init = 0; 
int flags = 0; 
int add_remove_pt = 0; 
float optical_flow_error[2][400]; 
float optical_flow_errorP[2][400]; 
float errX[400]; 
float errY[400]; 
 
CvPoint pt; 
static int flagg=0; 
 
 
void on_mouse( int event, int x, int y, int flags, void* param ) 
{ 
    if( !image ) 
        return; 
 
    if( image->origin ) 
        y = image->height - y; 
 
    if( event == CV_EVENT_LBUTTONDOWN ) 
    { 
        pt = cvPoint(x,y); 
        add_remove_pt = 1; 
        flagg=1; 
    } 
} 
 
 
int main( int argc, char** argv ) 
{ 
     
    FILE *ptr; 
    ptr=fopen("dataerr.dat","w+"); 
    CvCapture* capture = 0; 
 
    int counter1=0; 
 
    IplImage* image2 = 0; 
 
    float sumX=0; 
    float sumY=0; 
 
    float err_X; 
    float err_Y; 
 
    int XX=0; 
    int YY=0; 
 
    CvPoint ipt1; 
 
    int tempxx1=0; 
    int tempyy1=0; 
    int tempxx2=0; 
    int tempyy2=0; 
 
     
 
    char *imgFmt="pgm"; 
    char str1[100]; 
 
    /* Initailize the error array */ 
    for(int kk=0;kk<=400;kk++) 
    { 
        optical_flow_error[0][kk]=0; 
        optical_flow_errorP[0][kk]=0; 
        optical_flow_error[1][kk]=0; 
        optical_flow_errorP[1][kk]=0; 
    } 
 
    //capturing frame from video 
    capture = cvCaptureFromAVI("soccer_track.mpeg"); 
 
    cvNamedWindow( "KLT-Tracking Group_R", 0 ); 
    cvSetMouseCallback( "KLT-Tracking Group_R", on_mouse, 0 ); 
 
    if(add_remove_pt==1) 
    { 
        flagg=1; 
    } 
 
    for(;;) 
    { 
        IplImage* frame = 0; 
         
        int i, k, c; 
 
        //creating file name 
        sprintf(str1,"%d.%s",counter1,imgFmt); 
        err_X=0; 
        err_Y=0; 
        sumX=0; 
        sumY=0; 
 
        //decompressing the grab images 
 
        frame = cvQueryFrame( capture ); 
 
     
        if( !frame ) 
            break; 
 
        if( !image ) 
            //The first frame:to allocation some memories,and do somen initialization work 
        { 
            // allocate all the image buffers  
            image = cvCreateImage( cvGetSize(frame), 8, 3 ); 
            image->origin = frame->origin; 
            grey = cvCreateImage( cvGetSize(frame), 8, 1 );//make it grey 
            prev_grey = cvCreateImage( cvGetSize(frame), 8, 1 );//the previous frame in grey mode 
            pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );//pyramid frame 
            prev_pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );//previous pyramid frame 
            /* Define two pointers */ 
            points[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0])); 
            points[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0])); 
            status = (char*)cvAlloc(MAX_COUNT); 
            flags = 0; 
        } 
 
        cvCopy( frame, image, 0 );//frame->image 
 
        //converting the image into gray scale for further computation 
        cvCvtColor( image, grey, CV_BGR2GRAY ); 
         
        if( need_to_init ) 
        { 
             
            IplImage* eig = cvCreateImage( cvGetSize(grey), 32, 1 ); 
            IplImage* temp = cvCreateImage( cvGetSize(grey), 32, 1 ); 
            double quality = 0.01; 
            double min_distance = 10; 
         
 
            //using good features to track 
            count = MAX_COUNT; 
            cvGoodFeaturesToTrack( grey, eig, temp, points[1], &count, 
                                   quality, min_distance, 0, 3, 0, 0.04 ); 
            cvFindCornerSubPix( grey, points[1], count, 
            cvSize(win_size,win_size), cvSize(-1,-1), 
            cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03)); 
            cvReleaseImage( &eig ); 
            cvReleaseImage( &temp ); 
 
 
 
            add_remove_pt = 0; 
        } 
        else if( count > 0 ) 
        { 
            //using pyramidal optical flow method 
            cvCalcOpticalFlowPyrLK(  
                    prev_grey, grey,  
                    prev_pyramid, pyramid, 
                    points[0], points[1],  
                    count, cvSize(win_size,win_size),  
                    5, status,0, 
                    cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags ); 
             
            flags |= CV_LKFLOW_PYR_A_READY|CV_LKFLOW_PYR_B_READY; 
 
            for( i = k = 0; i < count; i++ ) 
            { 
                /* When need to add or remove the point */ 
                if( add_remove_pt ) 
                { 
 
                    double dx = pt.x - points[1][i].x; 
                    double dy = pt.y - points[1][i].y; 
                    /* Calulate the distance between the point you select and the point tracked  
                    if they are far from less than 5,stop the add or move action     
                    */ 
                    if( dx*dx + dy*dy <= 25 ) 
                    { 
                        add_remove_pt = 0; 
                        continue; 
                    } 
                } 
                 
                if( !status[i] )//if the point is not tracked correctly,pass! 
                    continue; 
                
                points[1][k++] = points[1][i]; 
 
                ipt1=cvPointFrom32f(points[1][i]);//get a point 
                 
            //calculating error here,initalize the error array 
                optical_flow_error[0][i]=ipt1.x; 
                optical_flow_error[1][i]=ipt1.y; 
 
 
            } 
            //taking average error for moving the window 
 
            for(int zz=0; zz<=count;zz++) 
                { 
                    errX[zz]=optical_flow_error[0][zz]- optical_flow_errorP[0][zz]; 
                    errY[zz]=optical_flow_error[1][zz]- optical_flow_errorP[1][zz]; 
 
                    sumX=sumX+errX[zz]; 
                    sumY=sumY+errY[zz]; 
 
                    optical_flow_errorP[0][zz]=optical_flow_error[0][zz]; 
                    optical_flow_errorP[1][zz]=optical_flow_error[1][zz]; 
 
                } 
 
                fprintf(ptr,"%d\n",count); 
                 
                err_X=sumX/count; 
                err_Y=sumY/count; 
 
            if(flagg==1) 
            { 
              int static startonce=0; 
 
            if(startonce==0) 
            { 
                 
             
            tempxx1=pt.x-20; 
            tempyy1=pt.y-20; 
 
            tempxx2=pt.x+20; 
            tempyy2=pt.y+20; 
 
            XX=pt.x; 
            YY=pt.y; 
 
            startonce=1; 
 
            } 
            if(err_X<3) 
            { 
                tempxx1=tempxx1+err_X; 
                tempyy1=tempyy1+err_Y; 
                tempxx2=tempxx2+err_X; 
                tempyy2=tempyy2+err_Y; 
 
                XX=XX+err_X; 
                YY=YY+err_Y; 
                fprintf(ptr,"%f %f\n",err_X,err_Y); 
            } 
 
            printf("\n%f",err_X); 
 
            //moving window 
 
            cvRectangle(image, cvPoint(tempxx1,tempyy1), cvPoint(tempxx2,tempyy2), cvScalar(255,0,0), 1); 
            cvCircle(image, cvPoint(XX,YY), 3, cvScalar(0,0,255), 1); 
        } 
            count = k; 
        } 
 
 
        if( add_remove_pt && count < MAX_COUNT ) 
        { 
            points[1][count++] = cvPointTo32f(pt); 
            cvFindCornerSubPix( grey, points[1] + count - 1, 1, 
                cvSize(win_size,win_size), cvSize(-1,-1), 
                cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03)); 
            add_remove_pt = 0; 
        } 
 
        CV_SWAP( prev_grey, grey, swap_temp ); 
        CV_SWAP( prev_pyramid, pyramid, swap_temp ); 
        CV_SWAP( points[0], points[1], swap_points ); 
        need_to_init = 0; 
 
       
        //writing image file to the file 
        //if(!cvSaveImage(str1,image)) printf("Could not save: %s\n",str1); 
        //storing in a video also 
  
         
        cvShowImage( "KLT-Tracking Group_R", image ); 
 
        c = cvWaitKey(100); 
        if( (char)c == 27 ) 
            break; 
        switch( (char) c ) 
        { 
        case 's': 
            need_to_init = 1; 
          } 
 
        counter1++; 
    } 
 
    cvReleaseCapture( &capture ); 
    cvDestroyWindow("KLT-Tracking Group_R"); 
 
    fcloseall(); 
     
    return 0; 
} 



