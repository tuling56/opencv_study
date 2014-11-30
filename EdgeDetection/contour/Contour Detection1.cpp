//Function：把加载图像进行二值化，寻找轮廓，再描绘轮廓
//Source: http://www.xuebuyuan.com/593448.html
//Status:
//Improve:
//Info:[11/15/2014 jmy]

#include "cv.h"
#include "highgui.h"
#include "cxcore.h"

using namespace std;


int main(int argc, char* argv[])
{
	int i = 0;
	int mode = CV_RETR_EXTERNAL;			//获取轮廓的模式
	int contours_num = 0;					//图像中获取轮廓的数目

	CvMemStorage *storage = cvCreateMemStorage(0);
											//创建并声明一个内存，提取轮廓时用的
	
	//-------------------------图像二值化-----------------------------//
	IplImage *src;							//声明一个图像指针
	src = cvLoadImage("samples/square/rect.png", 0);		//用来加载图像
	if (!src)
	{
		printf("fail to load\n");
		return -1;
	}

	cvThreshold(src, src, 128, 255, CV_THRESH_BINARY);
											//把加载图像二值化
	cvNamedWindow("BinImg");			//创建显示图像窗口
	cvMoveWindow("BinImg", 60, 60);		//设置显示窗口的位置
	cvShowImage("BinImg", src);			//图像显示
	cvWaitKey(1000);

	//-------------------------寻找轮廓-------------------------------//
	CvSeq *contour = 0;			//声明一个序列指针，用来存储第一个外接轮廓

	//寻找轮廓函数,返回轮廓数目
	contours_num = cvFindContours(src, storage, &contour, sizeof(CvContour), 
								mode, CV_CHAIN_APPROX_NONE);		

	printf("count of contour:%d\n", contours_num);//输出轮廓数目,怎么会有600多个？？？

	//me:统计轮廓数目
// 	int num=0;
// 	CvSeq*contour1=contour;
// 	while(contour1)
// 	{
// 		num++;
// 		contour1=contour1->h_next;
// 	}
// 	printf("true count of contour:%d\n", num);//输出轮廓数目,怎么会有600多个？？？


	//-------------------------绘制轮廓------------------------------//
	CvSeqReader reader;					//读序列
	int count = 0;
	if(contour!=0)
	{
		count = contour->total;			//获取轮廓点数
		cout<<"total dot count of contour ="<<count<<endl;	//输出点数
	}

	cvStartReadSeq(contour, &reader, 0);//初始化序列中的读取过程
	CvPoint pt1;						//声明一个二维坐标点
	IplImage* img = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
										//声明一个大小与输入图像一样，无符号3信道图像指针

	cvNamedWindow("ContourImg", 1);
	cvShowImage("ContourImg", img);

	for(i = 0; i< count; i++)
	{
		CV_READ_SEQ_ELEM(pt1, reader);				//顺序把点读入pt1中
		cvCircle(img, pt1, 1, CV_RGB(255, 0, 0));	//绘制圆点来构成轮廓
		cvShowImage("ContourImg", img);
		cvWaitKey(5);
	}

	cvWaitKey(0);

	//销毁窗口与释放所有内存
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&img);

	return 0;
}