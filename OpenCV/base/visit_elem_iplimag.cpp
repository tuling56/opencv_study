/* 本文参考：http://blog.csdn.net/xiaowei_cqu/article/details/7557063
 *	
 */

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

//cv1.x的访问方式
void visit_elem_Iplimage(char*picname)
{
	IplImage*img=cvLoadImage(picname,0);
	IplImage*result=cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	cvCopy(img,result);
	if (!img){
		printf("加载图片失败！\n");
	    exit(0);
	}
	cvNamedWindow("origin",CV_WINDOW_AUTOSIZE);
	cvShowImage("origin",img);
	
	/********单通道**********/
	if (result->nChannels==1)
	{
		//方式一:直接访问，速度快但容易出错
		uchar*data1=(uchar*)result->imageData;
		for (int i=0;i<result->height;i++)
		{
			for (int j=0;j<result->width;j++)
			{
				*data1=((uchar*)(result->imageData+i*result->widthStep))[j];

				if (*data1<50)//颜色反转
					*data1=255;
				else
					*data1=0;
			}
		}

		//方式二：指针访问，，我们可以通过定义指针来访问，进行了规整
		//uchar*data_p=(uchar*)result->imageData;
		//int step=result->width/sizeof(uchar);
		//uchar*data2;
		//for (int i=0;i<result->height;i++)
		//{
		//	for (int j=0;j<result->width;j++)
		//	{
		//		*data2=data_p[i*step+j];

		//		if (*data2>200)//颜色反转
		//			*data2=0;
		//		else
		//			*data2=255;
		//		
		//	}
		//}

		//方式三：cvGet2D方式
		double tmp;  
		for(int i=0;i<result->height;i++)  
			for(int j=0;j<result->width;j++)  
				tmp=cvGet2D(result,i,j).val[0];  


		//方式四：




	}
	
	/*******多通道*******/
	else
	{
		printf("多通道待完善中。。。。");
		
		//方式一：指针访问
		uchar* data=(uchar *)result->imageData;  
		int step = result->widthStep/sizeof(uchar);  
		int chanels = result->nChannels;
		uchar temp='a';
		uchar *b=&temp,*g=&temp,*r=&temp;  //指针一定要初始化吗
		for(int i=0;i<result->height;i++) 
		{
			for(int j=0;j<result->width;j++)
			{  
			
				*b=data[i*step+j*chanels+0];  
				*g=data[i*step+j*chanels+1];  
				*r=data[i*step+j*chanels+2]; 
				
				*b=(uchar)i;
				*g=(uchar)j;
				*r=temp;

			}  
		}
		
		//方式二:cvGet2D
		double tmpb,tmpg,tmpr;  
		for(int i=0;i<result->height;i++)
		{
			for(int j=0;j<result->width;j++)
			{  
				tmpb=cvGet2D(result,i,j).val[0];  
				tmpg=cvGet2D(result,i,j).val[1];  
				tmpr=cvGet2D(result,i,j).val[2];  
			} 
		}
		

	}

	cvNamedWindow("result",CV_WINDOW_AUTOSIZE);
	cvShowImage("result",result);

	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&img);
	cvReleaseImage(&result);


}