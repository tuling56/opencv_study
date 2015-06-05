/*
 *	说明：《基于二值化聚类的图像文字提取算法》论文的实现
 *  流程：灰度化→二值化→长线剔除→十字背景搜索→矩形文本框搜索→各个区域细校验
 */

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

int bin_cluser(char *filename)
{
	IplImage *orginimg=cvLoadImage(filename,1);

#pragma region 通道图像分离和合并
	//分布计算RGB通道的平均值
	CvScalar avgChannels=cvAvg(orginimg);
	double avgB=avgChannels.val[0];
    double avgG=avgChannels.val[1];
	double avgR=avgChannels.val[2];

	//cvSplit()分离出每个通道的具体值
	IplImage* bImg=cvCreateImage(cvGetSize(orginimg),IPL_DEPTH_8U,1);  
	IplImage* rImg=cvCreateImage(cvGetSize(orginimg),IPL_DEPTH_8U,1);    
	IplImage* gImg=cvCreateImage(cvGetSize(orginimg),IPL_DEPTH_8U,1);    
	

	cvSplit(orginimg,bImg,gImg,rImg,NULL);
	//cvSaveImage("bImg.png",bImg);
	//cvSaveImage("gImg.png",gImg);
	//cvSaveImage("rImg.png",rImg);

	

	//cvMerge()合并通道
	IplImage* rImg3=cvCreateImage(cvGetSize(orginimg),IPL_DEPTH_8U,3);    
	IplImage* gImg3=cvCreateImage(cvGetSize(orginimg),IPL_DEPTH_8U,3);    
	IplImage* bImg3=cvCreateImage(cvGetSize(orginimg),IPL_DEPTH_8U,3);

	cvMerge(0,gImg,rImg,0,bImg3);
	cvMerge(gImg,0,rImg,0,gImg3);
	cvMerge(bImg,gImg,0,0,rImg3);
	//cvSaveImage("bImg3.png",bImg3);
	//cvSaveImage("gImg3.png",gImg3);
	//cvSaveImage("rImg3.png",rImg3);

	
	cvWaitKey(0);
	cvReleaseImage(&bImg);
	cvReleaseImage(&gImg);
	cvReleaseImage(&rImg);

	cvReleaseImage(&bImg3);
	cvReleaseImage(&gImg3);
	cvReleaseImage(&rImg3);
#pragma endregion 通道图像分离和合并

#pragma region 灰度化
	double bval,gval,rval;  
	CvScalar grayval;
	IplImage*grayimg=cvCreateImage(cvSize(orginimg->width,orginimg->height),IPL_DEPTH_8U,1);
	for(int i=0;i<orginimg->height;i++)  
	{
		for(int j=0;j<orginimg->width;j++)
		{  
			bval=cvGet2D(orginimg,i,j).val[0];  
			gval=cvGet2D(orginimg,i,j).val[1];  
			rval=cvGet2D(orginimg,i,j).val[2]; 
			grayval.val[0]=(28*bval+151*gval+77*rval+128)/256;
			cvSet2D(grayimg,i,j,grayval);
		}  
	}

	cvSaveImage("grayimg.png",grayimg);
#pragma endregion 灰度化

#pragma region 二值化
	IplImage*binimg=cvCreateImage(cvSize(orginimg->width,orginimg->height),IPL_DEPTH_8U,1);
	cvThreshold(grayimg,binimg,180.0,255.0,CV_THRESH_BINARY);
	cvSaveImage("binimg.png",binimg);
	cvReleaseImage(&grayimg);
	//cvReleaseImage(&binimg);
#pragma endregion 二值化
	
#pragma region 长直线剔除
	IplImage *bin_noline=cvCreateImage(cvGetSize(orginimg),IPL_DEPTH_8U,1);
	for (int i=0;i<orginimg->height;i++)
	{
		int hor=0;//水平计数器
		CvScalar whitecolor;
		whitecolor.val[0]=0;
		double cur_val,up_val,down_val,left_val,right_val;

		for(int j=0;j<orginimg->width;j++)
		{			
			printf("\t第%3d行，第%3d列",i,j);
			cur_val=cvGet2D(binimg,i,j).val[0];
			//up_val=cvGet2D(binimg,i-1,j).val[0];
			down_val=cvGet2D(binimg,i+1,j).val[0];
		    //left_val=cvGet2D(binimg,i,j-1).val[0];
			//right_val=cvGet2D(binimg,i,j+1).val[0];

			//若到了行尾
			if (j==orginimg->width-1)
			 if (hor>24)
			 {
				 //设置j-count,....j-1数值为0
				 for (int k=j-hor;k<j;j++)
				 {
					 cvSet2D(bin_noline,i,k,whitecolor);
				 }
			 }

			//其它情况
			if(int(down_val)==0&&int(cur_val)==255) 
			{
				hor++; //当前点计数+1
			}
			else if (hor>24)
			{
				//设置j-count,....j-1数值为0
			   for (int k=j-hor;k<j;k++)
				{
					cvSet2D(bin_noline,i,k,whitecolor);
				}
               hor=0;
				
			}
		}
		
	}
#pragma endregion 长直线剔除
    
	
	cvReleaseImage(&orginimg);	
	return 0;
}

int longline_dele(char*filename)
{

	return 0;
}