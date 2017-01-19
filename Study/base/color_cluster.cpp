#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

/*
	该程序利用OpenCV中的K均值聚类函数Kmeans2对图像进行颜色聚类，达到分割的目的。
	编写此函数的目的是：Kmeans2函数的用法有些难掌握，参考资料少，尤其是对图像进行操作的例子少，我找了很久也找不到,
	找到的例子也运行不了，今天终于自己搞定了，想给大家分享一下，供大家参考，节省大家利用Kmeans2进行图像方面开发的时间
	,少走一些弯路。

	本例子对印章图像sample.bmp进行颜色聚类

	运行此程序一定要配置好OpenCv环境啊！！！


	 KMeans2
	 按照给定的类别数目对样本集合进行聚类 
 
	 void cvKMeans2( const CvArr* samples, int cluster_count,  CvArr* labels, CvTermCriteria termcrit );
	 samples 
	 输入样本的浮点矩阵，每个样本一行。 
	 cluster_count 
	 所给定的聚类数目 
	 labels 
	 输出整数向量：每个样本对应的类别标识 
	 termcrit 
	 指定聚类的最大迭代次数和／或精度（两次迭代引起的聚类中心的移动距离） 
	 函数 cvKMeans2 执行 k-means 算法 搜索 cluster_count 个类别的中心并对样本进行分类，输出 labels(i) 为样本 i 的类别标识。 
 */

//参数说明：nCuster为聚类的类数
int color_cluster(char *filename,int nCuster )
{
	IplImage* img=cvLoadImage(filename);

	int i,j;
	CvMat *samples=cvCreateMat((img->width)*(img->height),1,CV_32FC3);//创建样本矩阵，CV_32FC3代表32位浮点3通道（彩色图像）
	CvMat *clusters=cvCreateMat((img->width)*(img->height),1,CV_32SC1);//创建类别标记矩阵，CV_32SF1代表32位整型1通道

	int k=0;
	for (i=0;i<img->width;i++)
	{
		for (j=0;j<img->height;j++)
		{
			CvScalar s;
			//获取图像各个像素点的三通道值（BGR）
			s.val[0]=(float)cvGet2D(img,j,i).val[0];//B
			s.val[1]=(float)cvGet2D(img,j,i).val[1];//G
			s.val[2]=(float)cvGet2D(img,j,i).val[2];//R
			cvSet2D(samples,k++,0,s);//将像素点三通道的值按顺序排入样本矩阵
		}
	}

	//聚类类别数，后期可以通过学习确定分类数。
	cvKMeans2(samples,nCuster,clusters,cvTermCriteria(CV_TERMCRIT_ITER,100,1.0));//开始聚类，迭代100次，终止误差1.0

	//创建用于显示的图像，二值图像
	IplImage *binimg=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	
	//创建用于单独显示每个聚类结果的图像
	IplImage *cluster_img0=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	IplImage *cluster_img1=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	IplImage *cluster_img2=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);



	k=0;
	int val=0;
	float step=255/(nCuster-1);

	CvScalar bg={255,0,0,0};//背景设置为白色
	for (i=0;i<img->width;i++)
	{
		for (j=0;j<img->height;j++)
		{
			cvSet2D(cluster_img0,j,i,bg);
			cvSet2D(cluster_img1,j,i,bg);
			cvSet2D(cluster_img2,j,i,bg);
		}
	}
	for (i=0;i<img->width;i++)
	{
		for (j=0;j<img->height;j++)
		{
			val=(int)clusters->data.i[k++];
			CvScalar s;
			s.val[0]=255-val*step;//这个是将不同类别取不同的像素值，
			cvSet2D(binimg,j,i,s);	

			//将每个聚类进行分离
			switch(val)
			{
				case 0:
					cvSet2D(cluster_img0,j,i,s);break;//白色类
				case 1:
					cvSet2D(cluster_img1,j,i,s);break;//灰色类
				case 2:
					cvSet2D(cluster_img2,j,i,s);break;//黑色类
				default:
					break;
			}	
		
		}
    }


	cvSaveImage("PicVideo//cluster_img0.png",cluster_img0);
	cvSaveImage("PicVideo//cluster_img1.png",cluster_img1);
	cvSaveImage("PicVideo//cluster_img2.png",cluster_img2);


	cvNamedWindow( "原始图像", 1 ); 
	cvShowImage( "原始图像", img  );


	cvNamedWindow( "聚类图像", 1 ); 
	cvShowImage( "聚类图像", binimg  );
	cvSaveImage("PicVideo//clusterimg.png",binimg);
	cvWaitKey(0); //等待按键

	cvDestroyWindow( "原始图像" );
	cvDestroyWindow( "聚类图像" );

	cvReleaseImage( &img ); 
	cvReleaseImage( &binimg );
	cvReleaseImage(&cluster_img0);
	cvReleaseImage(&cluster_img1);
	cvReleaseImage(&cluster_img0);

	return 0;

}


