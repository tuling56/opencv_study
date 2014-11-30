#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include "SamFL.h"

#define DIF_NEG 0
#define DIF_POS 1

#define NOT_EDGE 0
#define EDGE 1

double edge_maxima[] = { 0 ,255 };
double edge_thresh;
int trackbar_value=20;
IplImage* grnt;
IplImage* edge;

double isEdge(double x , double *val)
{
double GrayDifNeg[8];
double GrayDifPos[8];
double Edge[2];
double Q[8];
double r[8];
int i;

for(i =0; i<8;i++){
Q[i]=val[i]-x;//算出Gray_Dif(i)
/* 计算出Membership Function */
GrayDifNeg[i]=sfl_fuzzyfier_s(Q[i], -255.0,edge_thresh);
GrayDifPos[i]=sfl_fuzzyfier_s(Q[i], edge_thresh, 255.0);
}

// Rules
r[0]=sfl_and(sfl_and(sfl_and(GrayDifNeg[0],GrayDifNeg[1]),GrayDifNeg[7]),sfl_and(sfl_and(GrayDifPos[3],GrayDifPos[4]),GrayDifPos[5]));
r[1]=sfl_and(sfl_and(sfl_and(GrayDifPos[0],GrayDifPos[1]),GrayDifPos[7]),sfl_and(sfl_and(GrayDifNeg[3],GrayDifNeg[4]),GrayDifNeg[5]));
r[2]=sfl_and(sfl_and(sfl_and(GrayDifNeg[1],GrayDifNeg[2]),GrayDifNeg[3]),sfl_and(sfl_and(GrayDifPos[5],GrayDifPos[6]),GrayDifPos[7]));
r[3]=sfl_and(sfl_and(sfl_and(GrayDifPos[1],GrayDifPos[2]),GrayDifPos[3]),sfl_and(sfl_and(GrayDifNeg[5],GrayDifNeg[6]),GrayDifNeg[7]));
r[4]=sfl_and(sfl_and(sfl_and(GrayDifNeg[2],GrayDifNeg[3]),GrayDifNeg[4]),sfl_and(sfl_and(GrayDifPos[0],GrayDifPos[7]),GrayDifPos[6]));
r[5]=sfl_and(sfl_and(sfl_and(GrayDifPos[2],GrayDifPos[3]),GrayDifPos[4]),sfl_and(sfl_and(GrayDifNeg[0],GrayDifNeg[7]),GrayDifNeg[6]));
r[6]=sfl_and(sfl_and(sfl_and(GrayDifNeg[0],GrayDifNeg[1]),GrayDifNeg[2]),sfl_and(sfl_and(GrayDifPos[4],GrayDifPos[5]),GrayDifPos[6]));
r[7]=sfl_and(sfl_and(sfl_and(GrayDifPos[0],GrayDifPos[1]),GrayDifPos[2]),sfl_and(sfl_and(GrayDifNeg[4],GrayDifNeg[5]),GrayDifNeg[6]));
/* 计算边界 */
Edge[EDGE]=sfl_or(r[0],sfl_or(r[1],sfl_or(r[2],sfl_or(r[3],sfl_or(r[4],sfl_or(r[5],sfl_or(r[6],r[7])))))));
Edge[NOT_EDGE]=sfl_not(Edge[EDGE]);
/* 解模糊 */
return sfl_defuzzyfier_coa(2,Edge,edge_maxima);
}


IplImage* FuzzyEdge(IplImage* image, float thresh )
{
IplImage *gray, *edge;
gray = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
edge = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
cvCvtColor(image, gray, CV_BGR2GRAY);//将图像转化为灰度图像
cvZero(edge);
cvNot( gray, edge );
uchar *data= (uchar *)gray->imageData;
uchar *Edata= (uchar *)edge->imageData;
int height = gray->height;
int width = gray->width;
int step = gray->widthStep;
double in[8];
edge_thresh=thresh;
int i,j;
for(i=1;i<height-1;i++){
for(j=1;j<width-1;j++){
/* 取出Q点的周围的八个点的灰度值 */
in[0]=data[(i-1)*step+(j+1)];//点7
in[1]=data[(i-1)*step+(j)];//点8
in[2]=data[(i-1)*step+(j-1)];//点1
in[3]=data[(i)*step+(j-1)];//点2
in[4]=data[(i+1)*step+(j-1)];//点3
in[5]=data[(i+1)*step+(j)];//点4
in[6]=data[(i+1)*step+(j+1)];//点5
in[7]=data[(i)*step+(j+1)];//点6
/* 然后有isEdge算出是否为边缘，传入参数第一个是Q点的灰度值，
* 第二个是周围八个点的灰度值*/
Edata[i*step+j]=isEdge(data[(i)*step+(j)],in);
}
}
cvReleaseImage( &gray );

return edge;
}


void on_trackbar(int pos)
{

edge=FuzzyEdge(grnt,trackbar_value-20);
/* FuzzyEdge是主要处理函数 */
cvShowImage("Fuzzy Edge", edge);
cvReleaseImage( &edge );
}


int main()
{
char Buf[512];

/* 先取得图片名称 */
puts("Enter Image File Name :");
gets(Buf);
/* 装载图片 */
grnt = cvLoadImage(Buf,1);
if(grnt){
cvNamedWindow("Orginal Image", 0);
cvShowImage("Orginal Image", grnt);
cvNamedWindow("Fuzzy Edge", 0);
cvCreateTrackbar( "Threshold", "Fuzzy Edge", &trackbar_value, 40, on_trackbar );
on_trackbar(0);
cvWaitKey(0);
cvDestroyWindow( "Orginal Image" );
cvReleaseImage( &grnt );
cvDestroyWindow( "Fuzzy Edge" );
return 0;
}
else{
puts("I can not open Image File !!! :( ");
cvWaitKey(0);
return -1;
}

}



