#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

//-----------------------------------【边缘检测】-----------------------------
//描述：
//-------------------------------------------------------------------------
void susan(char* path);
void krisch(char* path);
void histogram(char* path);
void roberts(char *path);
void laplace(char *path);
void laplace_mat(char*path);
void prewitt(char *path);
void canny(char *path);
void sobel(char *path);
void log(char *path);

int canny_trackbar(char*filename);//更详细地讲canny的用法,滑动条调节参数
int edge_trackbar( char*path);    //来自毛星云

//-----------------------------------【轮廓检测】-----------------------------
//描述：
//-------------------------------------------------------------------------
int contour1();
int contour2(char*path);
int contour4();    

//-----------------------------------【直线检测】-----------------------------
//描述：
//-------------------------------------------------------------------------
int stdhoughlinedete(char *filename);
int line_dete_2(char*filename);

//-----------------------------------【圆检测】-----------------------------
//描述：
//-------------------------------------------------------------------------
int hough_circle(char*filename);

//-----------------------------------【矩形检测】-----------------------------
//描述：
//-------------------------------------------------------------------------
int square_detection();


//-----------------------------------【二值化】-----------------------------
//描述：
//-------------------------------------------------------------------------
int binaryzation(char*path);
int adaptiveThreshold(char*path);


//-----------------------------------【图像滤波和平滑】-----------------------------
//描述：
//-------------------------------------------------------------------------
int meanshift_smooth_segmentation();






