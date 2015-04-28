/************************************************************************
 * 功能：图像细化
 * 来自于：http://blog.csdn.net/byxdaz/article/details/5642669
************************************************************************/

#include <cv.h>
#include <highgui.h>
//#include <opencv2/core/core.hpp>


//细化函数
void cvThin( IplImage* src, IplImage* dst, int iterations=1)
{
	CvSize size = cvGetSize(src);

	cvCopy(src, dst);
	int n = 0,i = 0,j = 0;
	for(n=0; n<iterations; n++)
	{
		IplImage* t_image = cvCloneImage(dst);
		for(i=0; i<size.height;  i++)
		{
			for(j=0; j<size.width; j++)
			{
				if(CV_IMAGE_ELEM(t_image,byte,i,j)==1)
				{
					int ap=0;
					int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
					int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
					if (p2==0 && p3==1)
					{
						ap++;
					}
					int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
					if(p3==0 && p4==1)
					{
						ap++;
					}
					int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
					if(p4==0 && p5==1)
					{
						ap++;
					}
					int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
					if(p5==0 && p6==1)
					{
						ap++;
					}
					int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
					if(p6==0 && p7==1)
					{
						ap++;
					}
					int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
					if(p7==0 && p8==1)
					{
						ap++;
					}
					int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
					if(p8==0 && p9==1)
					{
						ap++;
					}
					if(p9==0 && p2==1)
					{
						ap++;
					}
					if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
					{
						if(ap==1)
						{
							if(!(p2 && p4 && p6))
							{
								if(!(p4 && p6 && p8)) 
								{
									CV_IMAGE_ELEM(dst,byte,i,j)=0;
								}
							}
						}
					}

				}
			}
		}
		cvReleaseImage(&t_image);
		t_image = cvCloneImage(dst);
		for(i=0; i<size.height;  i++)
		{
			for(int j=0; j<size.width; j++)
			{
				if(CV_IMAGE_ELEM(t_image,byte,i,j)==1)
				{
					int ap=0;
					int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
					int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
					if (p2==0 && p3==1)
					{
						ap++;
					}
					int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
					if(p3==0 && p4==1)
					{
						ap++;
					}
					int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
					if(p4==0 && p5==1)
					{
						ap++;
					}
					int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
					if(p5==0 && p6==1)
					{
						ap++;
					}
					int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
					if(p6==0 && p7==1)
					{
						ap++;
					}
					int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
					if(p7==0 && p8==1)
					{
						ap++;
					}
					int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
					if(p8==0 && p9==1)
					{
						ap++;
					}
					if(p9==0 && p2==1)
					{
						ap++;
					}
					if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
					{
						if(ap==1)
						{
							if(p2*p4*p8==0)
							{
								if(p2*p6*p8==0)
								{
									CV_IMAGE_ELEM(dst, byte,i,j)=0;
								}
							}
						}
					}                    
				}

			}

		}            
		cvReleaseImage(&t_image);
	}

}


//细化函数入口
int thin(char*input)
{
	//if(argc!=2)
	//{
	//	return 0;
	//}

	IplImage *pSrc = NULL,*pDst = NULL,*pTmp = NULL;

	//传入一个灰度图像
	pSrc = cvLoadImage(input,CV_LOAD_IMAGE_GRAYSCALE);
	if(!pSrc)
	{
		return 0;
	}
	pTmp = cvCloneImage(pSrc);
	pDst = cvCreateImage(cvGetSize(pSrc),pSrc->depth,pSrc->nChannels);
	cvZero(pDst);
	cvThreshold(pSrc,pTmp,128,1,CV_THRESH_BINARY_INV);//做二值处理，将图像转换成0，1格式
	//cvSaveImage("c://Threshold.bmp",pTmp,0);
	cvThin(pTmp,pDst,8);//细化，通过修改iterations参数进一步细化
	cvNamedWindow("src",1);
	cvNamedWindow("dst",1);
	cvShowImage("src",pSrc);
	//将二值图像转换成灰度，以便显示
	int i = 0,j = 0;
	CvSize size = cvGetSize(pDst);
	for(i=0; i<size.height;  i++)
	{
		for(j=0; j<size.width; j++)
		{
			if(CV_IMAGE_ELEM(pDst,uchar,i,j)==1)
			{
				CV_IMAGE_ELEM(pDst,uchar,i,j) = 0;
			}
			else
			{
				CV_IMAGE_ELEM(pDst,uchar,i,j) = 255;
			}
		}
	}
	cvSaveImage("thin.bmp",pDst);
	cvShowImage("dst",pDst);
	cvWaitKey(0);
	cvReleaseImage(&pSrc);
	cvReleaseImage(&pDst);
	cvReleaseImage(&pTmp);
	cvDestroyWindow("src");
	cvDestroyWindow("dst");
	return 0;
} 