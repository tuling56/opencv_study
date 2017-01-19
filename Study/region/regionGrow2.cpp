/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	regionGrow.cpp
* Brief: 区域生长的限制条件是梯度
* Source:http://blog.csdn.net/sera_ph/article/details/7674847
* Status: 已调通
* Date:	[1/23/2015 jmy]
************************************************************************/
#include <windows.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <list>


#define INIT 0  
#define SEED -1  
#define CONT -2  
#define INVAL -3  

int color_templ[5 * 6 * 3] =
{
	0, 0, 0, 40, 40, 40, 80, 80, 80, 120, 120, 120, 160, 160, 160, 200, 200, 200,
	76, 0, 13, 113, 52, 0, 127, 91, 0, 188, 114, 0, 203, 140, 68, 236, 211, 190,
	0, 0, 121, 93, 0, 158, 91, 20, 237, 255, 61, 255, 170, 110, 240, 200, 161, 245,
	38, 88, 0, 77, 118, 46, 66, 130, 98, 112, 182, 46, 118, 197, 124, 174, 221, 178,
	19, 57, 96, 10, 98, 163, 0, 160, 171, 0, 242, 255, 59, 245, 255, 139, 247, 255
};

class g_point
{
  public:
	g_point();
	g_point(int x_i, int y_i);
	g_point(int x_i, int y_i, int lbl_i);
  public:
	int x;
	int y;
	int lbl;      //标签  
};

g_point::g_point(int x_i, int y_i)
{
	x = x_i;
	y = y_i;
	lbl = SEED;
}

g_point::g_point()
{
	x = 0;
	y = 0;
	lbl = INIT;
}

g_point::g_point(int x_i, int y_i, int lbl_i)
{
	x = x_i;
	y = y_i;
	lbl = lbl_i;
}

void regionGrowth(IplImage* img, int* p_mat, int x_g, int y_g, int threshold_g)
{

	int x,y;
	int gradient = 0;

	//设置前次边缘点  
	std::list<class g_point> cont_pts_pre;
	std::list<class g_point> * pcont_pts_pre = &cont_pts_pre;
	
	//设置边缘点  
	std::list<class g_point> cont_pts;
	std::list<class g_point> * pcont_pts = &cont_pts;

	//种子点+边缘点 = 下次的种子点  
	//上次的边缘点 = 前次边缘点（用于下次减少点数)   
	cont_pts_pre.push_back(class g_point(x_g, y_g, CONT));
	p_mat[y_g * img->width + x_g] = SEED;

	std::list<class g_point>::iterator iter_cont;
	std::list<class g_point>::iterator iter_prt;
	std::list<class g_point>::iterator iter_swap;

	while (!cont_pts_pre.empty())
	{
		//一轮生长  
		iter_cont = cont_pts_pre.begin();
		while (iter_cont != cont_pts_pre.end())
		{
			x = (*iter_cont).x;
			y = (*iter_cont).y;
			//         if( !(x-1<0 || y-1<0) )                       //#1  
			//         {     
			//             if(p_mat[(y-1)*img->width + (x-1)] == INIT)  
			//             {  
			//                 gradient = ((char*)(img->imageData + y*img->widthStep))[x] -   
			//                     ((char*)(img->imageData + (y-1)*img->widthStep))[x-1];  
			//                 if(abs(gradient) < threshold_g)      //满足阈值要求  
			//                 {  
			//                     cont_pts.push_back(class g_point(x-1, y-1, CONT));  
			//                     p_mat[(y-1)*img->width + (x-1)] = SEED;  
			//                 }  
			//                 else                                //不满足阈值要求  
			//                 {  
			//                     p_mat[(y-1)*img->width + (x-1)] = INVAL;  
			//                 }  
			//             }  
			//         }  
			if (!(x - 1 < 0))                                   //#2  
			{
				if (p_mat[(y)*img->width + (x - 1)] == INIT)
				{
					gradient = ((char*)(img->imageData + y*img->widthStep))[x] -
						        ((char*)(img->imageData + (y)*img->widthStep))[x - 1];
					if (abs(gradient) < threshold_g)      //满足阈值要求  
					{
						cont_pts.push_back(class g_point(x - 1, y, CONT));
						p_mat[(y)*img->width + (x - 1)] = SEED;
					}
					else                                     
					{
						p_mat[(y)*img->width + (x - 1)] = INVAL;
					}

				}
			}
			//         if( !(x-1<0 || y+1 >= img->height) )           //#3  
			//         {  
			//             if(p_mat[(y+1)*img->width + (x-1)] == INIT)  
			//             {  
			//                 gradient = ((char*)(img->imageData + y*img->widthStep))[x] -   
			//                     ((char*)(img->imageData + (y+1)*img->widthStep))[x-1];  
			//                 if(abs(gradient) < threshold_g)      //满足阈值要求  
			//                 {  
			//                     cont_pts.push_back(class g_point(x-1, y+1, CONT));  
			//                     p_mat[(y+1)*img->width + (x-1)] = SEED;  
			//                 }  
			//                 else                                //不满足阈值要求  
			//                 {  
			//                     p_mat[(y+1)*img->width + (x-1)] = INVAL;  
			//                 }  
			//                   
			//             }  
			//         }  
			if (!(y + 1 >= img->height))                       //#4  
			{
				if (p_mat[(y + 1)*img->width + (x)] == INIT)
				{
					gradient = ((char*)(img->imageData + y*img->widthStep))[x] -
						        ((char*)(img->imageData + (y + 1)*img->widthStep))[x];
					if (abs(gradient) < threshold_g)      //满足阈值要求  
					{
						cont_pts.push_back(class g_point(x, y + 1, CONT));
						p_mat[(y + 1)*img->width + (x)] = SEED;
					}
					else                                //不满足阈值要求  
					{
						p_mat[(y + 1)*img->width + (x)] = INVAL;
					}
				}
			}
			//         if( !(x+1>=img->width || y+1>=img->height) )    //#5  
			//         {  
			//             if(p_mat[(y+1)*img->width + (x+1)] == INIT)  
			//             {  
			//                 gradient = ((char*)(img->imageData + y*img->widthStep))[x] -   
			//                     ((char*)(img->imageData + (y+1)*img->widthStep))[x+1];  
			//                 if(abs(gradient) < threshold_g)      //满足阈值要求  
			//                 {  
			//                     cont_pts.push_back(class g_point(x+1, y+1, CONT));  
			//                     p_mat[(y+1)*img->width + (x+1)] = SEED;  
			//                 }  
			//                 else                                //不满足阈值要求  
			//                 {  
			//                     p_mat[(y+1)*img->width + (x+1)] = INVAL;  
			//                 }  
			//                   
			//             }  
			//         }  
			if (!(x + 1 >= img->width))                      //#6  
			{
				if (p_mat[(y)*img->width + (x + 1)] == INIT)
				{
					gradient = ((char*)(img->imageData + y*img->widthStep))[x] -
						((char*)(img->imageData + (y)*img->widthStep))[x + 1];
					if (abs(gradient) < threshold_g)      //满足阈值要求  
					{
						cont_pts.push_back(class g_point(x + 1, y, CONT));
						p_mat[(y)*img->width + (x + 1)] = SEED;
					}
					else                                //不满足阈值要求  
					{
						p_mat[(y)*img->width + (x + 1)] = INVAL;
					}

				}
			}
			//         if( !(x+1>=img->width || y-1<0) )              //#7  
			//         {  
			//             if(p_mat[(y-1)*img->width + (x+1)] == INIT)  
			//             {  
			//                 gradient = ((char*)(img->imageData + y*img->widthStep))[x] -   
			//                     ((char*)(img->imageData + (y-1)*img->widthStep))[x+1];  
			//                 if(abs(gradient) < threshold_g)      //满足阈值要求  
			//                 {  
			//                     cont_pts.push_back(class g_point(x+1, y-1, CONT));  
			//                     p_mat[(y-1)*img->width + (x+1)] = SEED;  
			//                 }  
			//                 else                                //不满足阈值要求  
			//                 {  
			//                     p_mat[(y-1)*img->width + (x+1)] = INVAL;  
			//                 }  
			//                   
			//             }  
			//         }  
			if (!(y - 1 < 0))                                   //#8  
			{
				if (p_mat[(y - 1)*img->width + (x)] == INIT)
				{
					gradient = ((char*)(img->imageData + y*img->widthStep))[x] -
						((char*)(img->imageData + (y - 1)*img->widthStep))[x];
					if (abs(gradient) < threshold_g)      //满足阈值要求  
					{
						cont_pts.push_back(class g_point(x, y - 1, CONT));
						p_mat[(y - 1)*img->width + (x)] = SEED;
					}
					else                                //不满足阈值要求  
					{
						p_mat[(y - 1)*img->width + (x)] = INVAL;
					}
				}
			}

			iter_cont++;
		}

		//将cont_pts中的点赋给cont_pts_pre  
		cont_pts_pre.clear();
		iter_swap = cont_pts.begin();
		while (iter_swap != cont_pts.end())
		{
			cont_pts_pre.push_back((*iter_swap));
			iter_swap++;
		}
		cont_pts.clear();
	}
	//结束while  

	cont_pts_pre.clear();
	cont_pts.clear();

	// 	delete pseed_pts;
	// 	delete pcont_pts_pre;
	// 	delete pcont_pts;
}


void display_mat(int * p_mat, int width, int height)
{
	IplImage* disp;
	disp = cvCreateImage(cvSize(width, height), 8, 3);
	cvZero(disp);

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			if (p_mat[h * width + w] == SEED)
			{
				((UCHAR *)(disp->imageData + h*disp->widthStep))[w * 3 + 0] = color_templ[0 * 18 + 0 * 3 + 0];
				((UCHAR *)(disp->imageData + h*disp->widthStep))[w * 3 + 1] = color_templ[0 * 18 + 0 * 3 + 1];
				((UCHAR *)(disp->imageData + h*disp->widthStep))[w * 3 + 2] = color_templ[0 * 18 + 0 * 3 + 2];
			}
			else if (p_mat[h * width + w] == INIT)
			{
				((UCHAR *)(disp->imageData + h*disp->widthStep))[w * 3 + 0] = color_templ[0 * 18 + 2 * 3 + 0];
				((UCHAR *)(disp->imageData + h*disp->widthStep))[w * 3 + 1] = color_templ[0 * 18 + 2 * 3 + 1];
				((UCHAR *)(disp->imageData + h*disp->widthStep))[w * 3 + 2] = color_templ[0 * 18 + 2 * 3 + 2];
			}
			else if (p_mat[h * width + w] == INVAL)
			{
				((UCHAR *)(disp->imageData + h*disp->widthStep))[w * 3 + 0] = color_templ[0 * 18 + 4 * 3 + 0];
				((UCHAR *)(disp->imageData + h*disp->widthStep))[w * 3 + 1] = color_templ[0 * 18 + 4 * 3 + 1];
				((UCHAR *)(disp->imageData + h*disp->widthStep))[w * 3 + 2] = color_templ[0 * 18 + 4 * 3 + 2];
			}
		}
	}
	cvNamedWindow("display");
	cvShowImage("display", disp);
	cvWaitKey(0);

	cvDestroyWindow("display");
	cvReleaseImage(&disp);
}


int regionGrow2(int argc, char* argv[])
{
	IplImage* img;
	img = cvLoadImage("samples\\200.png", CV_LOAD_IMAGE_ANYCOLOR);
	if (img==NULL){
		printf("加载图片失败\n");
		return -1;
	}

	//建立需要使用的单通道8位深图像
	IplImage* img_gray;
	img_gray = cvCreateImage(cvGetSize(img), img->depth, 1);
	cvSplit(img, NULL, NULL, img_gray, NULL); //在不同的通道上进行生长

	cvNamedWindow("original image", 1);
	cvShowImage("original image", img_gray);
	cvWaitKey(0);

	//建立所需的与图像同样宽高的整型数矩阵并赋初值
	int * pres_mat;
	pres_mat = new int[img_gray->width * img_gray->height];
	for (int i = 0; i < img_gray->width * img_gray->height; i++)
	{
		pres_mat[i] = INIT;
	}

	//开始进行区域生长
	regionGrowth(img_gray, pres_mat, 1, 1, 50);
	display_mat(pres_mat, img_gray->width, img_gray->height);

	return 0;
}

//功能测试区
//int main()
//{
//	regionGrow2(0, NULL);
//	return 0;
//}