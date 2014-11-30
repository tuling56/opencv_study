/************************************************************************
* Copyright(c) 2014 tuling56
*
* File:	firecorner.cpp
* Brief: 火灾尖角检测方法
* Source:http://blog.csdn.net/xiaowei_cqu/article/details/8515193
* Status: 为遍历图像像素做的准备工作
* Date:	[11/30/2014 jmy]
************************************************************************/

#include <opencv2/opencv.hpp>
using namespace cv;

void CheckFireAngle(Mat &img)
{
	imshow("origin",img);
	Mat copyImage,angGray,angleImg;

	int dilation_type = MORPH_RECT;
	int dilation_size=5;
	Mat element = getStructuringElement( dilation_type,
		Size( 2*dilation_size + 1, 2*dilation_size+1 ),
		Point( dilation_size, dilation_size ) );

	img.copyTo(copyImage);
	dilate(copyImage,copyImage,Mat());
	dilate(copyImage,copyImage,Mat());
	copyImage.copyTo(angGray);
	
	int nrows=angGray.rows;
	int ncols=angGray.cols;

	//cvtColor(copyImage,angGray,CV_BGR2GRAY);

	int flag=1;
	for(int i=0;i<nrows;i++)
	{
		uchar *pimg=copyImage.ptr<uchar>(i);
		uchar *pgray=angGray.ptr<uchar>(i);
		for(int j=0;j<ncols;j++)
		{
			if(pimg[j]>0)
			{
				if(i>0&&j>0)
				{
					uchar* ppgray=angGray.ptr<uchar>(i-1);
					if(ppgray[j]==0&&pgray[j-1]==0)
						pgray[j]=flag++;
					else if(ppgray[j]>0&&pgray[j-1]==0){
						pgray[j]=ppgray[j];
					}
					else if(pgray[j-1]>0&&ppgray[j]==0){
						pgray[j]=pgray[j-1];
					}
					else if(pgray[j-1]>0&&ppgray[j]>0&&pgray[j-1]==ppgray[j]){
						pgray[j]=pgray[j-1];
					}
					else{
						ppgray[j]=flag++;
						pgray[j]=ppgray[j];
						uchar tmp=pgray[j-1];
						for(int k=j-1;k>0;k--){
							if(pgray[k]==tmp)
								pgray[k]=pgray[j];
						}
					}
				}
				else if(j>0&&i==0){
					if(pgray[j-1]>0)
						pgray[j]=pgray[j-1];
					else
						pgray[j]=flag++;
				} 
				else if (i>0&&j==0){
					uchar *ppgray =angGray.ptr<uchar>(i-1);
					if(ppgray[j]>0)
						pgray[j]=ppgray[j];
					else
						ppgray[j]=flag++;
				}
				else{
					pgray[j]=flag++;
				}
			} 
		} 
	} 
	imshow("AngleGray",angGray);

	//标记完后画尖角的图
	Mat table = (Mat_<double>(10,3) << 0, 0, 1 , 0.5, 0.4, 0.25, 0.5, 0.5, 0.5, 0, 1, 0, 1, 0.8, 0, 1, 0.5, 1, 1, 0, 1 , 1, 0, 0, 1, 1, 1, 1, 1, 0 );
	cvtColor(angGray,angleImg,CV_GRAY2BGR);
	int channels=angleImg.channels();
	for(int i=0;i<nrows;i++)
	{
		uchar* pgray=angGray.ptr<uchar>(i);
		uchar* pang=angleImg.ptr<uchar>(i);
		for(int j=0,jcol=0;j<ncols,jcol<ncols*channels;j++,jcol+=channels)
		{
			uchar flagvalue=pgray[j];
			if(flagvalue>0)
			{
				for(int k=0;k<3;k++){
					int indexpix=(int)(flagvalue%10);
					double tmp= table.at<double>(indexpix,2-k);
					pang[jcol+k]=(uchar)tmp*255;
				} 
			}
		} 
	} 
	//cout<<flag<<endl;
	imshow("Taper angle",angleImg);
	waitKey();
}
