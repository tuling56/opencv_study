/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	morphy2.cpp
* Brief: 形态学膨胀/腐蚀，开/闭，角点检测，边缘检测和文本定位   OpenCV2.0结构
* Source:
* Status: 未完成，还存在问题
* Date:	[3/7/2015 jmy]
************************************************************************/
#include <stdio.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

int erode_dilate()
{
    Mat image=imread("PicVideo/cat.png");
    // 彩色转灰度
    cvtColor(image,image,CV_BGR2GRAY);
    // 阈值化
    threshold(image,image,255*(0.5),255,THRESH_BINARY);
    
    // 形态学操作
    // 如果把结构元素设置为Mat()，则将用默认的3*3的矩形结构元素
    Mat eroded;
    erode(image,eroded,Mat());
    Mat dilated;
    dilate(image,dilated,Mat());
	
	waitKey();
    return 0;
}

int open_close()
{
    Mat image=imread("PicVideo/cat.png");
    // 彩色转灰度
    cvtColor(image,image,CV_BGR2GRAY);
    // 阈值化
    threshold(image,image,255*(0.5),255,THRESH_BINARY);

    // 定义结构元素
    Mat se(5,5,CV_8U,Scalar(1));
    Mat closed;
    morphologyEx(image,closed,MORPH_CLOSE,se);
    Mat opened;
    morphologyEx(image,opened,MORPH_OPEN,se);

    waitKey();
	return 0;
}

int morphy_edge()
{
    Mat image=imread("PicVideo/cat.png");
    // 彩色转灰度
    cvtColor(image,image,CV_BGR2GRAY);
    Mat catEdge;
    morphologyEx(image,catEdge,MORPH_GRADIENT,Mat());

    // 阈值化
    threshold(catEdge,catEdge,40,255,THRESH_BINARY);
    namedWindow("catEdge");
	imshow("catEdge",catEdge);

    waitKey();
    return 0;
}

void drawOnImage(const Mat& binary,Mat& image)
{
	for(int i=0;i<binary.rows;i++)
	{
		// 获取行指针
		const uchar* data=binary.ptr<uchar>(i);
		for(int j=0;j<binary.cols;j++)
		{
			if(data[j]) //角点图像上的白点
				circle(image,Point(j,i),8,Scalar(0,255,0));// 画圈
		}
	}
}


int morphy_corner()
{
	Mat image=imread("PicVideo/car.png");
	// 定义结构元素
    Mat cross(5,5,CV_8U,Scalar(0));
    Mat diamond(5,5,CV_8U,Scalar(1));
    Mat square(5,5,CV_8U,Scalar(1));
    Mat x(5,5,CV_8U,Scalar(0));
    
    for(int i=0;i<5;i++)
    {
        cross.at<uchar>(2,i)=1;
        cross.at<uchar>(i,2)=1;

    }
    diamond.at<uchar>(0,0)=0;
    diamond.at<uchar>(0,1)=0;
    diamond.at<uchar>(1,0)=0;
    diamond.at<uchar>(4,4)=0;
    diamond.at<uchar>(3,4)=0;
    diamond.at<uchar>(4,3)=0;
    diamond.at<uchar>(4,0)=0;
    diamond.at<uchar>(4,1)=0;
    diamond.at<uchar>(3,0)=0;
    diamond.at<uchar>(0,4)=0;
    diamond.at<uchar>(0,3)=0;
    diamond.at<uchar>(1,4)=0;

    for(int i=0;i<5;i++){
        x.at<uchar>(i,i)=1;
        x.at<uchar>(4-i,i)=1;
    }

    Mat result;
    dilate(image,result,cross);
    erode(result,result,diamond);

	Mat result2;
	dilate(image,result2,x);
	erode(result2,result2,square);

	absdiff(result2,result,result);

    threshold(result,result,40,255,THRESH_BINARY);

	drawOnImage(result,image);
	namedWindow("corner");
	imshow("corner",image);
	
	waitKey();
	return 0;
}


/*
 *	功能：利用形态学的边缘检测进行文本定位
 *  参数：
 *  状态：效果还是不错的
 */
int morphy_apply_location()
{
	Mat cimage=imread("samples//text_dete//text_img0290.png");
    Mat image;
    cvtColor(cimage,image,CV_BGR2GRAY);
     
    Mat result;
    //检测竖直边缘
    morphologyEx(image,result,MORPH_GRADIENT,Mat(1,2,CV_8U,Scalar(1)));
    //阈值化
    threshold(result,result,255*(0.2),255,THRESH_BINARY);
    //水平方向闭运算
    morphologyEx(result,result,MORPH_CLOSE,Mat(1,20,CV_8U,Scalar(1)));
    //竖直方向闭运算
    morphologyEx(result,result,MORPH_CLOSE,Mat(10,1,CV_8U,Scalar(1)));

	namedWindow("apply");
	imshow("apply",result);
	//imwrite("dete.png", result);

    waitKey();
	return 0;
}

//功能测试区
//int main(int argc,char**argv)
//{
//    
//	//morphy_apply_location();
//	Mat src = imread("samples//canny.png");
//	Mat gray;
//	cvtColor(src, gray, CV_BGR2GRAY);
//	//阈值化
//	threshold(gray,gray, 255 * (0.2), 255, THRESH_BINARY);
//
//	//水平方向闭运算
//	morphologyEx(gray,gray, MORPH_CLOSE, Mat(1, 20, CV_8U, Scalar(1)));
//	//竖直方向闭运算
//	morphologyEx(gray,gray, MORPH_CLOSE, Mat(10, 1, CV_8U, Scalar(1)));
//
//	namedWindow("apply");
//	imshow("apply", gray);
//
//	cv::waitKey();
//	return 0;
//}