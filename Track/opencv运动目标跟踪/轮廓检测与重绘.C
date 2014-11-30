/**************提取模式. 
CV_RETR_EXTERNAL - 只提取最外层的轮廓 
CV_RETR_LIST - 提取所有轮廓，并且放置在 list 中 
CV_RETR_CCOMP - 提取所有轮廓，并且将其组织为两层的 hierarchy: 顶层为连通域的外围边界，次层为洞的内层边界。 
CV_RETR_TREE - 提取所有轮廓，并且重构嵌套轮廓的全部 hierarchy method 
*********************************/
#include "highgui.h"
#include "cv.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    int i=0;
    int mode=CV_RETR_EXTERNAL;//提取轮廓的模式
    int contours_num=0;//图像中提取轮廓的数目
    CvMemStorage *storage=cvCreateMemStorage(0);//设置轮廓时需要的存储容器
    CvSeq *contour =0;//设置存储提取的序列指针
    IplImage *img=cvLoadImage("lena.jpg",0);
    cvThreshold(img,img,128,255,CV_THRESH_BINARY);//二值化
    IplImage *pContourImg = cvCreateImage(cvGetSize(img),
        IPL_DEPTH_8U,3);
    cvNamedWindow("windows",1);
    cvShowImage("windows",img);//显示二值图
    /*cvFindContours查找物体轮廓*/
    mode=CV_RETR_EXTERNAL;//提取物体最外层轮廓
    contours_num=cvFindContours(img,storage,&contour,
        sizeof(CvContour),mode,CV_CHAIN_APPROX_NONE);
    cout<<"检测出的轮廓数目为："<<contours_num<<" "<<endl;

    /*逐点将轮廓画出*/
    CvSeqReader reader;//读序列
    int count=0;
    if (contour!=0)
    {
        count=contour->total;
        cout<<count<<endl;
    }

    cvStartReadSeq(contour,&reader,0);
    CvPoint pt1;
    CvScalar color=CV_RGB(255,255,255);
    cvNamedWindow("contour",1);
    cvShowImage("contour",pContourImg);
    for (i=0;i<count;i++)//逐点绘图法
    {
        CV_READ_SEQ_ELEM(pt1,reader);//读出一个点
        cvCircle(pContourImg,pt1,1,color);//画出一个点（就是一个以一个像素大小为半径的圆）
        cvShowImage("contour",pContourImg);
        cvWaitKey(5);
    }
    cvWaitKey(0);

    cvReleaseImage(&img);
    cvReleaseImage(&pContourImg);
    cvReleaseMemStorage(&storage);
    return 0;
} 



