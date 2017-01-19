/*===============================================//
功能：RGB读取并保存
时间：02/23/2012 SkySeraph
//===============================================*/
#include "iostream"
#include <fstream>
using namespace std;

#include "cv.h"
#include "highgui.h"

//#pragma comment(lib,"highgui.lib")
//#pragma comment(lib,"cv.lib")
//#pragma comment(lib,"cvaux.lib")
//#pragma comment(lib,"cxcore.lib")

int ReadSaveRGB()
{
    /* ①
    IplImage*img =  cvLoadImage("rgb.bmp",-1);
    if(img==NULL)
        return 0;
    CvScalar p;
    ofstream outfile("d:\\rgb.txt");
    outfile<<"图像宽和高:"<<img->width<<"*"<<img->height<<endl;
    for(int i=0;i<img->height;i++)
    {
        for(int j=0;j<img->width;j++)
        {
            p = cvGet2D(img,i,j);
            outfile<<p.val[0] <<" "<<p.val[1]<<" "<<p.val[2] <<" "<<endl;
        }    
    }    
*/
///*  ②
    IplImage* img =  cvLoadImage("PicVideo//rgb.png",-1);
    CvScalar p;
    ofstream outfile1("b.txt");
    ofstream outfile2("g.txt");
    ofstream outfile3("r.txt");
    outfile1<<"图像宽和高:"<<img->width<<"*"<<img->height<<endl;
    outfile1<<"图像B值"<<endl;
    outfile2<<"图像宽和高:"<<img->width<<"*"<<img->height<<endl;
    outfile2<<"图像G值"<<endl;
    outfile3<<"图像宽和高:"<<img->width<<"*"<<img->height<<endl;
    outfile3<<"图像R值"<<endl;
    //cvFlip(img);
    for(int i=0;i<img->width;i++)
    {
        for(int j=0;j<img->height;j++)
        {
            p = cvGet2D(img,j,i);//采用的cvGet2D的方法
            outfile1<<p.val[0]<<" ";
            outfile2<<p.val[1]<<" ";
            outfile3<<p.val[2]<<" ";
        }
        outfile1<<endl;
        outfile2<<endl;
        outfile3<<endl;
    }   

    return 0;
}