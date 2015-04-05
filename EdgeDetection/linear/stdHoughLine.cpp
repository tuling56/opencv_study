/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	line_dete.cpp
* Brief: 提供有4种直线检测方法的实现
* Source:
* Status: 
* Date:	[3/13/2015 jmy]
************************************************************************/

#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>
#include <string.h>



//-----------------------------------【检测模块一】------------------------------
//	描述:opencv直线检测程序,在此仅作为参考
//  说明：Switch between standard and probabilistic Hough transform by changing 
//        "#if 1" to "#if 0" and back 
//------------------------------------------------------------------------------
#pragma region 检测方法一

static int stdHoughLine(char*filename,bool isStd)
{
	IplImage* src;
	if ((src = cvLoadImage(filename, 0)) != 0)
	{
		IplImage* edgeimg = cvCreateImage(cvGetSize(src), 8, 1);
		IplImage* color_dst = cvCreateImage(cvGetSize(src), 8, 3);
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* lines = 0;
		cvCanny(src, edgeimg, 50, 200, 3);  //先进行边缘检测
		cvCvtColor(edgeimg, color_dst, CV_GRAY2BGR);

		int i;
		if (isStd)
		{
			lines = cvHoughLines2(edgeimg, storage, CV_HOUGH_STANDARD, 1, CV_PI / 180, 100, 0, 0);
			for (i = 0; i < MIN(lines->total, 100); i++)
			{
				float* line = (float*)cvGetSeqElem(lines, i);
				float rho = line[0];
				float theta = line[1];
				CvPoint pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a*rho, y0 = b*rho;
				pt1.x = cvRound(x0 + 1000 * (-b));
				pt1.y = cvRound(y0 + 1000 * (a));
				pt2.x = cvRound(x0 - 1000 * (-b));
				pt2.y = cvRound(y0 - 1000 * (a));
				cvLine(color_dst, pt1, pt2, CV_RGB(255, 0, 0), 1, 8);
			}
		}
		else
		{ 
			lines = cvHoughLines2(edgeimg, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 80, 30, 10);
			for (i = 0; i < lines->total; i++)
			{
				CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
				cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 1, 8);
			}

		}

		cvNamedWindow("Source", 1);
		cvNamedWindow("HoughLine", 1);
		cvShowImage("Source", src);
		cvShowImage("HoughLine", color_dst);

		cvWaitKey(0);
		return 0;
	}
}
#pragma endregion 检测方法一


//-----------------------------------【检测方法二】------------------------------
//	描述:检测直线，并统计某一直线指定区域内的直线条数，画矩形标注
//  说明：Switch between standard and probabilistic Hough transform by changing
//		  "#if 1" to "#if 0" and back 
//-------------------------------------------------------------------------------
#pragma region 检测方法二

int callinenum(CvSeq*lines,int y_value,int height,int varinace,CvPoint*end_line);
int line_dete_2(char*filename)
{
	IplImage* src = cvLoadImage( filename, 0 );
	if (!src){
		printf("加载图像:%s失败！\n",filename);
		return -1;
	}
	IplImage* dst;
	IplImage* color_dst;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;
	int i;
	dst = cvCreateImage( cvGetSize(src), 8, 1 );
	color_dst = cvCreateImage( cvGetSize(src), 8, 3 );
	cvCanny( src, dst, 50, 200, 3 );
	cvCvtColor( dst, color_dst, CV_GRAY2BGR );//为什么要把灰度图转换为彩色图？
#if 0
	lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );

	for( i = 0; i < MIN(lines->total,100); i++ )
	{
		float* line = (float*)cvGetSeqElem(lines,i);
		float rho = line[0];
		float theta = line[1];
		CvPoint pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );
	}
#else
	lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 100, 20 );
	printf("满足条件的直线共有%d条\n",lines->total);
	int num_line=0;
	int height=10;//固定宽度为10的下方区域

	for( i = 0; i < lines->total; i++ )
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
	   
		//对检测直线的处理
		char variace=5;
		if (abs(line[0].x-line[1].x)<variace) //竖直直线
			cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 1, CV_AA, 0 );

		if (abs(line[0].y-line[1].y)<variace) //水平直线
		  	cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 1, CV_AA, 0 );
	   
	    //画矩形框
	   //CvPoint*end_line=NULL;
	   //num_line=callinenum(lines,line[0].y,height,variace,end_line);//统计区域直线的条数
	   //if (num_line>10)//对直线数的判断
	   //{
		  // //这条直线下height的高度内是文本区域，画矩形标注
		  // cvRectangle(color_dst,line[0],end_line[1],CV_RGB(0,255,0),1);
	   //}


	}

#endif

	cvNamedWindow( "原始图像（灰度化)", 1 );
	cvNamedWindow( "(水平和竖直)直线检测结果", 1 );
	cvShowImage( "原始图像（灰度化)", src );
	cvShowImage( "(水平和竖直)直线检测结果", color_dst );
	//cvSaveImage("（水平和竖直)直线检测结果.png",color_dst);

	cvWaitKey(0);

	return 0;
}

//统计每条直线下固定height内的直线条数
int callinenum(CvSeq*lines,int y_value,int height,int varinace,CvPoint*end_line)
{
  
	int line_number=0;

	for(int i = 0; i < lines->total; i++ )
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);//指向一条直线
		if (line[0].y<y_value+height)
		{	
			line_number++;
			end_line=line;//带回区域内最后一条直线
	    }
		
	}
	return line_number;;//返回直线的条数
}
#pragma endregion 检测方法二


//------------------------------【检测模块3】------------------------------
//	描述:检测直线,统计水平和竖直直线的条数并直线标号及文件保存
//-------------------------------------------------------------------------

#pragma region 检测方法三

int callinenum_hor(CvSeq*lines,int y_value,int height,char variace,int hor_hit[]);
int callinenum_ver(CvSeq*lines,int x_value,int width,char variace,int ver_hit[]);

FILE *fp;

int line_dete_3(char*filename)
{
	
	IplImage* src = cvLoadImage( filename, 0 );
	IplImage* dst;
	IplImage* color_dst;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;

	if( !src ){
		printf("图像加载失败！\n");
		return -1;
	}

	//文件记录操作
	if ((fp=fopen("hit.txt","w"))==NULL){
		printf("打开文件失败");
		exit(1);
	}

	//图像标号操作
	CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_PLAIN,0.8,0.8,0.5,1.3,8);


	dst = cvCreateImage( cvGetSize(src), 8, 1 );
	color_dst = cvCreateImage( cvGetSize(src), 8, 3 );

	cvCanny( src, dst, 50, 200, 3 );
	cvCvtColor( dst, color_dst, CV_GRAY2BGR );//为什么要把灰度图转换为彩色图？
	cvRectangle(color_dst,cvPoint(0,0),cvPoint(25,25),CV_RGB(0,0,255),1);//矩形测试

	lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 20, 100, 20 );
	
	char variace=10;//容许范围
	int line_number=0;
	int line_number_limit=5;
	int arealimit=30;//固定宽度为10的下方区域

	//统计水平和竖直直线的条数
	int hor_num=0;
	int ver_num=0;
	int hor_ver_num=0;

	//记录水平和竖直直线被击中
	int hor_hit[400];
	int ver_hit[400];

	fprintf(fp,"所有直线(包括倾斜和水平)共有%d条\n",lines->total);

	for( int i = 0; i < lines->total; i++ )
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);//获得每条直线
		
		//水平直线检测
		if (abs(line[0].y-line[1].y)<variace)
		{  	
			hor_num++;
			hor_ver_num++;

			fprintf(fp,"-------第%d条直线是水平直线-----------\n",i);
			cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 1, CV_AA, 0 );
			
			//直线标号
			char number[10];
			sprintf(number,"%d",i);
			cvPutText(color_dst,number,line[0],&font,CV_RGB(32,69,207));

			line_number=callinenum_hor(lines,line[0].y,arealimit,variace,hor_hit);

			if (line_number>line_number_limit)
			{
				for(int j=0;j<400;j++)
				{
				   if(i==hor_hit[j]){
					   fprintf(fp,"     第%d条水平直线被击中了.....\n",i);
					   break;   
				   }
				}

				cvRectangle(color_dst,line[0],cvPoint(line[1].x,line[1].y+arealimit),CV_RGB(0,255,0),1);				
			}
		}
		 //竖直直线检测
		if (abs(line[0].x-line[1].x)<variace)
		{
			ver_num++;
			hor_ver_num++;
			fprintf(fp,"-------第%d条直线是竖直直线-----------\n",i);
			cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 1, CV_AA, 0 );
			
			//直线标号
			char number[10];
			sprintf(number,"%d",i);
			cvPutText(color_dst,number,line[0],&font,CV_RGB(184,32,207));

			line_number=callinenum_ver(lines,line[0].x,arealimit,variace,ver_hit);

			if (line_number>line_number_limit)
			{
				for (int j=0;j<400;j++)
				{
					if (i==ver_hit[j]){
					   fprintf(fp,"    第%d竖直直线已经被画在了某个矩形区域内。。。。\n",i);
					   break;
					}
				}

				cvRectangle(color_dst,line[0],cvPoint(line[1].x+arealimit,line[1].y),CV_RGB(0,255,0),1);	
			}
		}

	}
	fprintf(fp,"水平和竖直直线共有%d\n",hor_ver_num);
	fprintf(fp,"水平直线共有%d\n",hor_num);
	fprintf(fp,"竖直直线共有%d\n",ver_num);

	fclose(fp);

	cvNamedWindow( "直线检测（input）", 1 );
	cvNamedWindow( "直线检测（output）", 1 );

	cvShowImage( "直线检测（input）", src );
	cvShowImage( "直线检测（output）", color_dst );

	//cvSaveImage("line_detected.jpg",color_dst);

	cvWaitKey(0);

	return 0;
}


/* function:统计每条直线下固定arealimit宽度或长度下的直线条数
 *	paras:
 *		CvSeq*lines-->所有的直线
 *		y_value-->要统计点的y坐标值
 *		x_value-->要统计点的x坐标值
 *		height-->要统计的垂直区域大小
 *		width-->要统计的水平区域大小
 */
int callinenum_hor(CvSeq*lines,int y_value,int height,char variace,int hor_hit[])
{
    int number=0;
	for(int i = 0; i < lines->total; i++ )
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
		if (abs(line[0].y-line[1].y)<variace)
			if ((y_value<line[0].y)&&(line[0].y<y_value+height))
			  {
				  number++;
				  //记录下这条被击中的水平直线标号
				  hor_hit[i]=i;
				  fprintf(fp,"   该水平直线下固定区域的其它水平直线标号：%d\n",i);
			  }
	}
	return number;
}

int callinenum_ver(CvSeq*lines,int x_value,int width,char variace,int ver_hit[])
{
	int number=0;
	for(int i = 0; i < lines->total; i++ )
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
		if (abs(line[0].x-line[1].x)<variace)
		  if ((x_value<line[0].x)&&(line[0].x<x_value+width))
			{
				number++;
				//记录西这条被击中的竖直直线标号
				ver_hit[i]=i;
				 fprintf(fp,"   该竖直直线左右的其它竖直直线标号：%d\n",i);
		    }

	}
	return number;
}
#pragma endregion 检测方法三


//------------------------------【检测模块四】------------------------------
//	描述:把用标准hough变换检测出的最强直线转换成ax+by+c=0形
//-------------------------------------------------------------------------
#pragma region 检测方法四

int line_dete_4(char *filename)
{
	IplImage* imgTest = cvLoadImage(filename,0);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;

	lines = cvHoughLines2( imgTest, storage, CV_HOUGH_STANDARD,1,CV_PI/180, 5, 0, 0 );
	float* line = (float*)cvGetSeqElem(lines,0);
	float rho = line[0];          // distance from (0,0) to the line
	float theta = line[1];        // angle between x-axis and the normal to the line

	double thetaDegree = theta/3.1415*180;

	IplImage* imgRes = cvCreateImage(cvSize(imgTest->width,imgTest->height),IPL_DEPTH_8U,3);
	cvSetZero(imgRes);

	double a,b,c;                // ax+by+c=0
	if (theta>CV_PI/2)
	{
		theta = CV_PI-theta;
		if (theta>CV_PI/4)
		{
			a = 1/tan(theta);
			b = -1;
			c = rho/sin(theta);
		}
		else
		{
			a = 1;
			b = -tan(theta);
			c = rho/cos(theta);
		}
	}
	else
	{
		if (theta>CV_PI/4)
		{
			a = -1/tan(theta);
			b = -1;
			c = rho/sin(theta);
		}
		else
		{
			a = -1;
			b = -tan(theta);
			c = rho/cos(theta);
		}
	}

	if (abs(a)>abs(b))        // using x = -b/a*y-c/a
	{
		for (int h=0;h<imgRes->height;h++)
		{
			int w = -b/a*h-c/a;
			if (w<imgRes->width && w>=0)
			{
				imgRes->imageData[h*imgRes->widthStep+3*w] = 255;
			}
		}
	}
	else                    // using y = -a/b*x-c/b
	{
		for (int w=0;w<imgRes->width;w++)
		{    

			int h = -a/b*w-c/b;
			if (h<imgRes->height && h>=0)
				imgRes->imageData[h*imgRes->widthStep+3*w] = 255;
		}
	}
	cvSaveImage("PicVideo/houghline.jpg",imgRes);
	cvReleaseImage(&imgRes);
	cvReleaseImage(&imgTest);
	return 0;
}
#pragma endregion 检测方法四

//功能测试区
//int main(int argc,char**argv)
//{
//	stdHoughLine("samples//road.jpg", false);
//
//    std::cin.get();
//	return 0;
//}