/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	contour3_4.cpp
* Brief: 轮廓单点获得和轮廓等级学习,使用的OpenCV1.0结构
* Source:
* Status: 
* Date:	[4/3/2015 jmy]
************************************************************************/


//Function：寻找轮廓并讲解CvSeq的用法，cvGetSeqElem
//Source: http://blog.csdn.net/augusdi/article/details/9000276
//Status:基于OpenCV1
//Improve:
//Info:[11/15/2014 jmy]

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>
#include <stdio.h>


int contour3( int argc, char** argv )
{
	//声明IplImage指针
	IplImage* pImg = NULL; 
	IplImage* pContourImg = NULL;
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contour = 0;
	int mode = CV_RETR_EXTERNAL;
	unsigned th=0;
	if( argc == 3)
		if(strcmp(argv[2], "all") == 0)
			mode = CV_RETR_CCOMP; //内外轮廓都检测 

	cvNamedWindow("src", 1);
	cvNamedWindow("contour",1);
	
	//以灰度图像载入，然后强制将灰度图转化为彩色图
	if( (pImg = cvLoadImage("samples//200.png", 0)) != 0 )
	{
		cvShowImage( "src", pImg );
		pContourImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,3);
		//copy source image and convert it to BGR image
		cvCvtColor(pImg, pContourImg, CV_GRAY2BGR);//只不过是将每个通道都赋成相同的灰度值
		cvCanny(pImg, pImg, 50, 150, 5); //在边缘检测图像的基础上再寻找轮廓
		//cvThreshold(gray, gray, 100, 255, CV_THRESH_BINARY_INV);
		//cvSmooth(gray, gray, 2);

		int Num=cvFindContours( pImg, storage, &contour, sizeof(CvContour), mode, CV_CHAIN_APPROX_SIMPLE);
		std::cout<<"The number of Contours is:"<<Num<<std::endl;
		
		for(;contour!=0;contour=contour->h_next)
		{
			printf("***************************************************\n");
			for(int i=0;i<contour->total;i++)
			{
				CvPoint* p=(CvPoint*)cvGetSeqElem(contour,i);//依次取得轮廓中的每个点
				printf("p->x=%d,p->y=%d\n",p->x,p->y); 
			}
			//将轮廓画出 
			cvDrawContours(pContourImg, contour, CV_RGB(255,0,0), CV_RGB(0, 255, 0), 0, 2, 0); 
		}
	}
	else
	{
		cvDestroyWindow( "src" );
		cvDestroyWindow( "contour" );
		cvReleaseMemStorage(&storage);
		return -1;
	}


	cvShowImage( "contour", pContourImg );
	cvWaitKey(0);

	cvDestroyWindow( "src" );
	cvDestroyWindow( "contour" );

	cvReleaseImage( &pImg ); 
	cvReleaseImage( &pContourImg ); 
	cvReleaseMemStorage(&storage);
	return 0;
}



//Function：提取轮廓两种方法对比及绘制轮廓'最大等级'分析 
//Source:http://blog.csdn.net/augusdi/article/details/9000893
//Status:采用OpenCV1.0结构
//Improve:
//Info:[11/16/2014 jmy]

int contour4()
{
	IplImage* img = cvLoadImage("samples/contour/contour.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* img_temp = cvCreateImage(cvGetSize(img), 8, 1);

	cvThreshold(img, img, 128, 255, CV_THRESH_BINARY);

	CvMemStorage* mem_storage = cvCreateMemStorage(0);
	CvSeq *first_contour = NULL, *c = NULL;

	//////////////////////////////////////////////////////////////////////////    
	//1、方法一    
	cvNamedWindow("contour1");
	cvCopyImage(img, img_temp);
	double t = (double)cvGetTickCount();

	//默认的提取方法是CV_RETR_LIST                                                                
	cvFindContours(img_temp, mem_storage, &first_contour, 88, CV_RETR_CCOMP);
	cvZero(img_temp); //置黑   
	cvDrawContours(img_temp, first_contour, cvScalar(100), cvScalar(250), 2);

	t = (double)cvGetTickCount() - t;
	cvShowImage("contour1", img_temp);
	printf("run1 = %gms\n", t / (cvGetTickFrequency()*1000.));
	cvClearMemStorage(mem_storage);

	//////////////////////////////////////////////////////////////////////////    
	// 2、方法二    
	cvNamedWindow("contour2");
	cvCopyImage(img, img_temp);
	t = (double)cvGetTickCount();

	CvContourScanner scanner = cvStartFindContours(img_temp, mem_storage);
	while (cvFindNextContour(scanner));
	first_contour = cvEndFindContours(&scanner);

	cvZero(img_temp);
	cvDrawContours(img_temp, first_contour, cvScalar(100), cvScalar(100), 1);

	t = (double)cvGetTickCount() - t;
	cvShowImage("contour2", img_temp);
	printf("run2 = %gms\n", t / (cvGetTickFrequency()*1000.));
	cvClearMemStorage(mem_storage);

	cvReleaseImage(&img);
	cvReleaseImage(&img_temp);

	cvWaitKey();
	cvDestroyAllWindows();

	return 0;
	/************************************************************************/
	/* 经测试 run1 = 16.1431ms run2 = 15.8677ms (参考)
	不过可以肯定这两中算法时间复杂度是相同的                                     */
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////    
	// 上述两种方法完成了对轮廓的提取,如想绘制轮廓都得配合cvDrawContours来使用    
	// 而cvDrawContours 函数第5个参数为 max_level 经查ICVL含义如下:    
	//    
	// 绘制轮廓的最大等级。
	// 如果为0，绘制单独的轮廓。
	// 如果为1，绘制轮廓及在其后的相同的级别下轮廓。    
	// 如果为2，绘制所有同级轮廓及所有低一级轮廓，诸此种种。
	// 如果值为负数，函数不绘制同级轮廓，但会升序绘制直到级别为abs(max_level)-1的子轮廓。    
	//    
	// 相信好多读者初次都无法理解等级的含义,而且测试时候输入>=1 的整数效果几乎一样    
	// 只有提取轮廓时候的提取模式设为 CV_RETR_CCOMP CV_RETR_TREE 时这个参数才有意义    
	//    
	// 经查FindContours 函数里面这样介绍提取模式(mode)的这两个参数:    
	// CV_RETR_CCOMP - 提取所有轮廓，并且将其组织为两层的 hierarchy: 顶层为连通域的外围边界，次层为洞的内层边界。     
	// CV_RETR_TREE - 提取所有轮廓，并且重构嵌套轮廓的全部 hierarchy     
	//     
	// 下面用第一种方法进行测试    

	cvNamedWindow("contour_test");
	cvNamedWindow("contour_raw");
	img = cvLoadImage("samples/contour/contour.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cvShowImage("contour_raw", img);
	cvThreshold(img, img, 128, 255, CV_THRESH_BINARY);
	img_temp = cvCloneImage(img);
	cvFindContours(
		img_temp,
		mem_storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_TREE           //#1 需更改区域    
		);

	cvZero(img_temp);
	cvDrawContours(
		img_temp,
		first_contour,
		cvScalar(100),
		cvScalar(100),
		1                       //#2 需更改区域    
		);
	cvShowImage("contour_test", img_temp);
	/************************************************************************/
	/* (1, 2) = (CV_RETR_CCOMP, 1)  如图1
	(1, 2) = (CV_RETR_CCOMP, 2)  如图2
	(1, 2) = (CV_RETR_TREE, 1)   如图3
	(1, 2) = (CV_RETR_TREE, 2)   如图4
	(1, 2) = (CV_RETR_TREE, 6)   如图5
	经分析CV_RETR_CCOMP 只把图像分为两个层次,顶层和次层,一等级轮廓只匹配与其最接近
	的内侧轮廓即2等级
	CV_RETR_TREE 则从轮廓外到内按等级1 - n 全部分配
	CV_RETR_LIST 全部轮廓均为1级                        */
	/************************************************************************/

	cvWaitKey();
	cvReleaseImage(&img);
	cvReleaseImage(&img_temp);
	cvReleaseMemStorage(&mem_storage);
	cvDestroyAllWindows();
	return 0;
}


//Function：把加载图像进行二值化，寻找轮廓，再描绘轮廓
//Source: http://www.xuebuyuan.com/593448.html
//Status:
//Improve:
//Info:[11/15/2014 jmy]

int contour5()
{
	IplImage *src= cvLoadImage("samples/square/rect.png", 0);		
	if (!src){
		printf("fail to load\n");
		return -1;
	}
	cvThreshold(src, src, 128, 255, CV_THRESH_BINARY);

	cvNamedWindow("BinImg");			
	cvMoveWindow("BinImg", 60, 60);		
	cvShowImage("BinImg", src);			
	cvWaitKey();

	//-------------------------寻找轮廓-------------------------------//
	int mode = CV_RETR_EXTERNAL;			       //获取轮廓的模式
	CvMemStorage *storage = cvCreateMemStorage(0); //创建并声明一个内存，提取轮廓时用的
	CvSeq *contour = 0;			                   //声明一个序列指针，用来存储第一个外接轮廓

	//寻找轮廓函数,返回轮廓数目
	int contours_num = cvFindContours(src, storage, &contour, sizeof(CvContour),mode, CV_CHAIN_APPROX_NONE);
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
	CvSeqReader reader;					
	int count = 0;
	if (contour != 0){
		count = contour->total;			//获取轮廓点数
		std::cout << "total dot count of contour =" << count << std::endl;
	}

	cvStartReadSeq(contour, &reader, 0);//初始化序列中的读取过程
	CvPoint pt1;						//声明一个二维坐标点
	IplImage* img = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);//声明一个大小与输入图像一样，无符号3信道图像指针
	
	cvNamedWindow("ContourImg", 1);
	cvShowImage("ContourImg", img);

	for (int i = 0; i < count; i++)
	{
		CV_READ_SEQ_ELEM(pt1, reader);				//顺序把点读入pt1中
		cvCircle(img, pt1, 1, CV_RGB(255, 0, 0));	//绘制圆点来构成轮廓
		cvShowImage("ContourImg", img);
		cvWaitKey(5);
	}

	cvWaitKey(0);

	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&img);

	return 0;
}