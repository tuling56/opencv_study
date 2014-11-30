//Function：基于opencv的图像边缘检测
//Detail:包括canny、Log、sobel、susan、prewitt、roberts、laplace、krisch等9中方法。
//Source:
//Status:
//Improve:
//Info:[11/18/2014 jmy]

#include "../detection.h"
#include <cmath>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

using namespace cv;

void susan(char* path)
{
     int height ,width ,step ,channels ;
     int i,j,k,same ,max,min,thresh,sum;
     uchar*data0,*data1 ;
     //char *filename="result.bmp";
     IplImage* Img,*nimg; //声明IplImage指针
     Img = cvLoadImage( path,0); //单通道形式加载
	 if(!Img)
		return;
     nimg = cvCreateImage(cvGetSize(Img),8,1);
	 height = Img->height;
     width = Img->width;
     step  = Img->widthStep/sizeof(uchar);
     channels = Img->nChannels;
     data0   = (uchar*)Img->imageData;
     data1 =  (uchar*)nimg->imageData;

     printf("Processing a %d X %d image with %d channels\n",width,height,channels);
     int OffSetX[37] = { -1, 0, 1,
                         -2,-1, 0, 1, 2,
                         -3,-2,-1, 0, 1, 2, 3,
                         -3,-2,-1, 0, 1, 2, 3,
                         -3,-2,-1, 0, 1, 2, 3,
                         -2,-1, 0, 1, 2,
                           -1, 0, 1 };
     int OffSetY[37] = { -3,-3,-3,
                        -2,-2,-2,-2,-2,
                        -1,-1,-1,-1,-1,-1,-1,
                        0, 0, 0, 0, 0, 0, 0,
                         1, 1, 1, 1, 1, 1, 1,
                         2, 2, 2, 2, 2,
                         3, 3, 3 };

     max = min = data0[0];
     //for(i=0;i<height;i++)
     // for(j=0;j<width;j++)
      //{
     // if(data0[i*step+j]>max) max = data0[i*step+j];
     //if(data0[i*step+j]<min)   min = data0[i*step+j];
     //   }
    for(i=3;i<height-3;i++)
       for(j=3;j<width-3;j++)
       {
          same =0;sum = 0;
          for(k=0;k<37;k++)
          {
            sum+=data0[(i+OffSetY[k])*step+(j+OffSetX[k])];
            thresh = sum/37;
            if(abs( data0[(i+OffSetY[k])*step+(j+OffSetX[k])]-data0[i*step+j])<=thresh)
                same++;
            if(same<18)
                data1[i*step+j] = 255;
            else
                data1[i*step+j] = 0;
     }
   }

	cvNamedWindow("SUSAN",1);  
	cvShowImage("SUSAN",nimg);    
	cvWaitKey(0);                      
	cvDestroyWindow("SUSAN"); 
    cvReleaseImage(&nimg);	

}

void krisch(char* path)
{
	IplImage *src;
	src=cvLoadImage(path,0); //0 单通道加载
	if(!src)
		return;
    IplImage *dst = cvCloneImage(src);
	int x,y;
	float a,b,c,d;
	float p1,p2,p3,p4,p5,p6,p7,p8,p9; 
	uchar* ps = (uchar*)src->imageData ; //ps为指向输入图片数据的指针
	uchar* pd = (uchar*)dst->imageData ; //pd为指向输出图片数据的指针
	int w = dst->width;
	int h = dst->height;
	int step = dst->widthStep;

	for(x = 0;x<w-2;x++)      //取以（x+1，y+1)为中心的9个邻域像素  1 4 7
	{                                                            // 2 5 8
		for(y = 0;y<h-2;y++)                                     // 3 6 9
		{  
			p1=ps[y*step+x];
			p2=ps[y*step+(x+1)];
			p3=ps[y*step+(x+2)];
			p4=ps[(y+1)*step+x];
			p5=ps[(y+1)*step+(x+1)];
			p6=ps[(y+1)*step+(x+2)];
			p7=ps[(y+2)*step+x];
			p8=ps[(y+2)*step+(x+1)];
			p9=ps[(y+2)*step+(x+2)];//得到(i+1,j+1)周围九个点的灰度值
			a = fabs(float(-5*p1-5*p2-5*p3+3*p4+3*p6+3*p7+3*p8+3*p9));    //计算4个方向的梯度值
			b = fabs(float(3*p1-5*p2-5*p3+3*p4-5*p6+3*p7+3*p8+3*p9));
			c = fabs(float(3*p1+3*p2-5*p3+3*p4-5*p6+3*p7+3*p8-5*p9));
			d = fabs(float(3*p1+3*p2+3*p3+3*p4-5*p6+3*p7-5*p8-5*p9));
			a = (a>b)?a:b;                                         //取各个方向上的最大值作为边缘强度
			a = (a>c)? a:c;
			a = (a>d)?a:d;
			pd[(y+1)*step+(x+1)] = a;
		}
	} 
	cvNormalize(dst,dst,0,255,CV_MINMAX); //归一化处理
	
	cvNamedWindow("krisch",1);      
	cvShowImage("krisch",dst); 
	cvWaitKey(0);                  
	cvDestroyAllWindows();  
    cvReleaseImage(&dst);
    cvReleaseImage(&src);
}



void roberts(char *path)
{
    IplImage *src=cvLoadImage(path,0);//自己修改了，原来的加载参数是-1，即加载原始通道数，但显示图像的时候出现异常
	if(!src)
		return;
	IplImage *robert=cvCloneImage(src);
	int x,y,i,w,h;
	int gx,gy;
	uchar* ptr=(uchar*) (robert->imageData );
	int ptr1[4]={0};
	int indexx[4]={0,1,1,0};
	int indexy[4]={0,0,1,1};
    w=robert->width;
	h=robert->height;
	for(y=0;y<h-1;y++)
		for(x=0;x<w-1;x++)
		{
			for(i=0;i<4;i++)
			{                                                             
				ptr1[i]= *(ptr+(y+indexy[i])*robert->widthStep+x+indexx[i]);
				
			}
			gx=abs(ptr1[0]-ptr1[2]);
			gy=abs(ptr1[1]-ptr1[3]);
			gx=(gx>gy?gx:gy);
	    	gx=(int)sqrt((double)gx*gx+(double)gy*gy);
			//二值化
			/*if (gx>50)
				gx=255;
			else 
			   gx=0;  
			*/
			*(ptr+y*robert->widthStep+x)=gx; 

		}
	cvNamedWindow("Original Image",1);
	cvShowImage("Original Image",src);	
	cvNamedWindow("robert",0);
	cvShowImage("robert",robert);
	cvWaitKey(0);
	cvReleaseImage ( &src );
	cvReleaseImage ( &robert );
	cvDestroyAllWindows();
}


void laplace(char *path)
{
    IplImage *src = cvLoadImage(path,-1);
	if(!src)
		return;
    IplImage *d_Image = NULL; 
    d_Image = cvCloneImage(src); 
    cvLaplace(src,d_Image,3); //拉普拉斯变换
	
	cvNamedWindow("Original Image",1);
	cvShowImage("Original Image",src);
	cvNamedWindow("Laplace",1);     
	cvShowImage("Laplace",d_Image);   
	
	cvWaitKey(0);                      
	
	cvDestroyWindow("Laplace");   
    cvReleaseImage(&src);
	cvReleaseImage(&d_Image);
	cvReleaseImage(&src);	
}

void laplace_mat(char*path)
{
	//【0】变量的定义
	Mat src,src_gray,dst, abs_dst;

	//【1】载入原始图  
	src = imread(path);  //工程目录下应该有一张名为1.jpg的素材图

	//【2】显示原始图 
	imshow("【原始图】图像Laplace变换(2.0)", src); 

	//【3】使用高斯滤波消除噪声
	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

	//【4】转换为灰度图
	cvtColor( src, src_gray, CV_RGB2GRAY );

	//【5】使用Laplace函数
	Laplacian( src_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT );

	//【6】计算绝对值，并将结果转换成8位
	convertScaleAbs( dst, abs_dst );

	//【7】显示效果图
	imshow( "【效果图】图像Laplace变换(2.0)", abs_dst );

	waitKey(0); 

}



void prewitt(char *path)
{
	 //-------prewitt算子 适用灰度图 ----
	IplImage *src=cvLoadImage(path,-1);
	if(!src)
		return;
    IplImage *gray = cvCreateImage(cvGetSize(src),8,1) ;
    if (src->nChannels == 1) 
    {
	   cvCopy( src, gray);
	}
	else
    {
		cvCvtColor(src,gray,CV_BGR2GRAY);//色彩空间转换，转换类型为CV_BGR2GRAY
    }
	IplImage *dst=NULL;
	dst = cvCreateImage(cvGetSize(gray),8,1);
	//定义prewitt算子的模板
	float prewittx[9] =         
	{
		-1,0,1,
		-1,0,1,
		-1,0,1
	};
	float prewitty[9] = 
	{
		1,1,1,
		0,0,0,
		-1,-1,-1
	};
	CvMat px;   
	px = cvMat(3,3,CV_32F,prewittx);   
	CvMat py;
	py = cvMat(3,3,CV_32F,prewitty);

	//为输出图像申请空间
	IplImage *dstx = cvCreateImage(cvGetSize(gray),8,1); 
	IplImage *dsty = cvCreateImage(cvGetSize(gray),8,1);

	//对图像使用模板，自动填充边界 cvCvtColor(src,gray,CV_RGB2GRAY);
	cvFilter2D(gray,dstx,&px,cvPoint(-1,-1)); 
	cvFilter2D(gray,dsty,&py,cvPoint(-1,-1));

	//计算梯度，范数为2，注意学习指针的使用方法
	int i,j,temp;
	float tempx,tempy;  //定义为浮点型是为了避免sqrt函数引起歧义
	uchar* ptrx = (uchar*) dstx->imageData;
	uchar* ptry = (uchar*) dsty->imageData;
	for(i = 0;i<src->width;i++)
	{
		for(j = 0;j<src->height;j++)
		{
			tempx = ptrx[i+j*dstx->widthStep];   //tempx,tempy表示的是指针所指向的像素
			tempy = ptry[i+j*dsty->widthStep]; 
			temp = (int) sqrt(tempx*tempx+tempy*tempy);
			dst->imageData[i+j*dstx->widthStep] = temp;
		}
	}	 
	cvReleaseImage(&dstx);
	cvReleaseImage(&dsty);
	cvNamedWindow("Original Image",1);
	cvShowImage("Original Image",src);
	cvNamedWindow("Prewitt",1);    
	cvShowImage("Prewitt",dst);    
	cvWaitKey(0);                     
	cvDestroyAllWindows();  
    cvReleaseImage(&dst);
	cvReleaseImage(&src);
	cvReleaseImage(&gray);
}


void canny(char *path)
{
    IplImage *img;
	img=cvLoadImage(path,0); //0 单通道加载
	if(!img)
		return;
	IplImage *imgCanny;
	imgCanny=cvCreateImage(cvSize(img->width,img->height),img->depth,img->nChannels);
	cvCanny(img,imgCanny,100,60);
	
	cvNamedWindow("Original Image",1);
	cvShowImage("Original Image",img);
	
	cvNamedWindow("Canny Image",1);
	cvShowImage("Canny Image",imgCanny);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&imgCanny);
	cvDestroyAllWindows();   

}


void sobel(char *path)  
{
	IplImage *img;
	img=cvLoadImage(path,-1); //-1保持原来的颜色通道
	if(!img)
		return;
	IplImage *img16S,*imgSobel,*imgSobelx,*imgSobely;
	CvScalar mean;
	img16S=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_16S,img->nChannels);
	
	imgSobel=cvCreateImage(cvSize(img->width,img->height),img->depth,img->nChannels);
	imgSobelx=cvCreateImage(cvSize(img->width,img->height),img->depth,img->nChannels);
	imgSobely=cvCreateImage(cvSize(img->width,img->height),img->depth,img->nChannels);
	cvSobel(img,img16S,1,0,3);
	cvConvertScaleAbs(img16S,imgSobelx,1);
	cvSobel(img,img16S,0,1,3);
	cvConvertScaleAbs(img16S,imgSobely,1);
	cvAdd(imgSobelx,imgSobely,imgSobel);	//使用两个方向的梯度绝对值和近似梯度模

	cvNamedWindow("Original Image",1);
	cvShowImage("Original Image",img);
	cvNamedWindow("Sobel Image",1);
	cvShowImage("Sobel Image",imgSobel);
	cvWaitKey(0);
	
	cvReleaseImage(&img);
	cvReleaseImage(&imgSobel);
	cvReleaseImage(&imgSobelx);
	cvReleaseImage(&imgSobely);
	cvReleaseImage(&img16S);
	cvDestroyAllWindows();   
}

void log(char *path)  
{  
     
	IplImage *img;
	img=cvLoadImage(path,-1); //-1保持原来的颜色通道
	if(!img)
		return;
	IplImage *imgLoG,*img16S;
	CvScalar mean;
	img16S=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_16S,img->nChannels);
	imgLoG=cvCreateImage(cvSize(img->width,img->height),img->depth,img->nChannels);
	cvSmooth(img,img,CV_GAUSSIAN,5,5);	//高斯模糊
	cvLaplace(img,img16S,3);			//拉普拉斯算子
	cvConvertScaleAbs(img16S,imgLoG,1);

	//double min_val = 0;double max_val = 0;
	//cvMinMaxLoc(imgLoG,&min_val,&max_val);   //取图像中的最大最小像素值
	//cvNormalize(imgLoG,imgLoG,0,255,CV_MINMAX); //归一化处理

	cvNamedWindow("Original Image",1);
	cvShowImage("Original Image",img);
	cvNamedWindow("LoG Image",1);
	cvShowImage("LoG Image",imgLoG);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&imgLoG);
	cvReleaseImage(&img16S);
	cvDestroyAllWindows();   
}  


