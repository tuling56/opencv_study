// CompareImg1.cpp : 定义控制台应用程序的入口点。
//

/*2012-3-7 功能：比较两幅图的纹理特征，从而判断两幅图像的相似度*/

//#include "stdafx.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\cxcore.h>

int GetTexturePara(CvMat* paraMatri[4],double *texturePara);//
double GetVar(double ar[4]);//求纹理参数的方差
double GetMean(double ar[4]);//求纹理参数的均值
int GetGLCM(CvMat * ImgMat,double* texturePara);//由共生矩阵提取各个纹理参数，
//并通过GetTexturePara函数调用GetVar和GetMean函数,计算各个参数的均值和方差
double comparePara(double* textureArray1,double* textureArray2);//比较两个图像矩阵的纹理参数，返回代表相似度的值
double ComputeTextureDiff(IplImage *pImg1,IplImage *pImg2);//比较两幅图像的相似度，首先调用GetGLCM函数，然后调用函数comparePara

int CompareImg()
{

	IplImage *pImg1 = cvLoadImage("PIC/zx/(1)pp.png",-1);//加载两幅图像
	IplImage *pImg2 = cvLoadImage("PIC/zx/(3)Canny.png",-1);

	if (!pImg1 && !pImg2 )
	{
		return -1;
	}


	double dDiff = 0.0; //两幅图的差别
	dDiff = ComputeTextureDiff(pImg1,pImg2);
   printf("两幅图像的差别为：%f",dDiff);
   cvWaitKey(0);
   

	return 0;
}


double comparePara(double* textureArray1,double* textureArray2)
{

	double paraDis[16];
	double sumPara=0;
	double tempmax= 0;
	for(int i=0;i<16;i++)
	{
		tempmax=textureArray1[i] > textureArray2[i] ? textureArray1[i] : textureArray2[i];
		if (tempmax==0)
		{
			tempmax = 1;
		}
		paraDis[i]= (textureArray1[i]-textureArray2[i])*(textureArray1[i]-textureArray2[i])/(tempmax*tempmax);

		sumPara += paraDis[i];

	}
	sumPara =1- sqrt(sumPara)/16;//相似度 值越大相似度越大

	return sumPara;
}
double GetMean(double ar[4])//计算均值，GetGLCM函数中会用到
{
	double sumValue=0;
	for (int i=0;i<4;i++)
	{
		sumValue += ar[i];
	}
	return (sumValue/4.0);
}
double GetVar(double ar[4])//计算方均值，GetGLCM函数中会用到
{
	double sumValue=0;
	for (int i=0;i<4;i++)
	{
		sumValue += ar[i]*ar[i];
	}
	return (sumValue/4.0);
}
int GetTexturePara(CvMat* paraMatri[4],double *texturePara)
{
	double tempArray[4]={0};
	/*static double texturePara[16]={0};*/
	int textureNum=0;
	for(int paraNum=0;paraNum<8;paraNum++)
	{

		for (int i=0;i<4;i++)
		{
			tempArray[i] = cvGetReal1D(paraMatri[i],paraNum);   //矩阵每一列都表示相同纹理参数，只是在不同的角度下而已

		}
		*(texturePara+textureNum)= GetMean(tempArray);
		*(texturePara+textureNum)=GetVar(tempArray) - (*(texturePara+textureNum))* (*(texturePara+textureNum));
		textureNum++;

	}
	return 0;
}

int GetGLCM(CvMat * ImgMat,double* texturePara)//生成共生矩阵并求出相关的纹理参数
{
	int i,j,m,n,dim;
	double  minvalue = 0.0,maxvalue = 0.0;
	int imgWidth,imgHeight;
	imgWidth = ImgMat->width;
	imgHeight = ImgMat->height;
	CvMat* ImgMatUchar = cvCreateMat(imgHeight,imgWidth,CV_8UC1);//图像灰度值范围0-255
	cvMinMaxLoc(ImgMat,&minvalue,&maxvalue);//寻找最大值最小值

	//--------------------以下两行代码实现图像归一化，将像素值归一化到0-255
	cvConvertScale(ImgMat,ImgMat,1,-minvalue);//线性变换转换数组
	cvConvertScale(ImgMat,ImgMat,255/(maxvalue-minvalue),0);
	cvConvert(ImgMat,ImgMatUchar);

	//--------------------为了提高计算速度，必须对图像进行量化------------
	for(j=0;j<imgHeight;j++)
		for(i=0;i<imgWidth;i++)
			for (n=0;n<16;n++)//像素分级
			{
				uchar temp = CV_MAT_ELEM(*ImgMatUchar,uchar,j,i);
				if (n*16<=temp&&(temp<=n*16+15))
				{
					*((uchar*)CV_MAT_ELEM_PTR(*ImgMatUchar,j,i)) = n;
				}
			}

			//------------------计算共生矩阵，取距离为1，角度为0,45,90,135------------
			//create comatrix
			CvMat* CoMatri[4];//数组套数组，openCV中这个用法!!!! 矩阵套数组
			for (dim =0;dim<4;dim++)
			{
				CoMatri[dim]=cvCreateMat(16,16,CV_64FC1);
				cvZero(CoMatri[dim]);
			}
			for (m=0;m<16;m++)//分16个灰度等级，16*16的共生矩阵，共生矩阵的大小由图像的灰度级决定
			{
				for (n=0;n<16;n++)
				{
					//printf("%d",n*m);
					for(j=0;j<imgHeight;j++)//图像大小
					{
						for (i=0;i<imgWidth;i++)
						{
							uchar temp = CV_MAT_ELEM(*ImgMatUchar,uchar,j,i);//图像中某个像素
							double tempMatValue;

							//统计0度灰度共生矩阵
							if (i<(imgWidth-1)&&(temp == m)&&(CV_MAT_ELEM((*ImgMatUchar),uchar,j,(i+1)) == n))
						 {
							 // printf("%d,%d,%d,%d\n",i,j,m,n);

							 tempMatValue = cvGetReal2D(CoMatri[0],m,n);
							 cvSetReal2D(CoMatri[0],m,n,(tempMatValue+1.0));//统计个数，没遇到一次满足条件的加1
							 cvSetReal2D(CoMatri[0],n,m,(tempMatValue+1.0));
						 }
							//统计45度灰度共生矩阵


							if((i<(imgWidth-1))&&(j>0)&&(temp == m)&&(CV_MAT_ELEM(*ImgMatUchar,uchar,j-1,i+1) == n))//报错的原因所在，条件判断是有顺序的！

						 {
							 tempMatValue = cvGetReal2D(CoMatri[1],m,n);
							 cvSetReal2D(CoMatri[1],m,n,(tempMatValue+1.0));//统计个数，没遇到一次满足条件的加1
							 cvSetReal2D(CoMatri[1],n,m,(tempMatValue+1.0));
						 }
							//统计90度灰度共生矩阵
							if (j<(imgHeight-1)&&temp==m&&(CV_MAT_ELEM(*ImgMatUchar,uchar,j+1,i)==n))
						 {
							 tempMatValue = cvGetReal2D(CoMatri[2],m,n);
							 cvSetReal2D(CoMatri[2],m,n,(tempMatValue+1.0));//统计个数，没遇到一次满足条件的加1
							 cvSetReal2D(CoMatri[2],n,m,(tempMatValue+1.0));
						 }


							//统计135度灰度共生矩阵
							if (i<(imgWidth-1)&&j<(imgHeight-1)&&temp==m&&(CV_MAT_ELEM(*ImgMatUchar,uchar,j+1,i+1)==n))//判断要有顺序，必须是先判断当前元素是否有下一个相邻元素，否则不进行判断
						 {
							 tempMatValue = cvGetReal2D(CoMatri[3],m,n);
							 cvSetReal2D(CoMatri[3],m,n,(tempMatValue+1.0));//统计个数，没遇到一次满足条件的加1
							 cvSetReal2D(CoMatri[3],n,m,(tempMatValue+1.0));
						 }


						}
					}
					//---------------共生矩阵的对角线上的值------
					if (m == n)
					{
						for (dim =0;dim<4;dim++)//错误之一！！！！ 进入死循环
						{
							double tempMatValue1 = cvGetReal2D(CoMatri[dim],m,n);
							cvSetReal2D(CoMatri[dim],m,n,(tempMatValue1*2));
						}
					}


				}
			}


			// -------------------对共生矩阵进行归一化处理------------------
			CvMat * NormalizedMat =cvCreateMat(16,16,CV_64FC1);
			for (dim=0;dim<4;dim++)
			{
				CvScalar sumValue = cvSum(CoMatri[dim]);
				cvSet(NormalizedMat,sumValue);//初始化矩阵
				cvDiv(CoMatri[dim],NormalizedMat,CoMatri[dim]);//a./b->c 错误2！！！！！
			}

			//------------------计算共生矩阵的特征能量、熵、惯性矩、相关等--------------
			double energy[4] = {0};//能量
			double entropy[4] = {0};
			double angleMoment[4] ={0};
			double inertia[4] = {0};
			double localSmooth[4] ={0};//局部平稳
			double Pmax[4] ={0};//最大概率
			double DIS[4] = {0};//相异
			double DifEntropy[4] = {0};//差熵
			double SumEntropy[4] = {0};//和熵
			double SumIndexVal= 0;
			double DifIndexVal = 0;
			double IndexVal[2*16-1] ={0};//用于存储以x+y=k为下标的共生矩阵中的值
			double IndexValDif[16] ={0};//用于存储以|x-y|=k为下标的共生矩阵中的值
			double sumVar[4] = {0};//和熵的方差
			double difVar[4] = {0};//差熵的方差
			//相关性中的均值
			//由于共生矩阵为对角阵，各个行与列的均值相等
			double dMean = 0.0;//求每一列或者行的均值
			double dSum[16] = {0.0};//记录每个行或者列的和
			double dStdDev = 0.0;//求每一列或者行的方差
			double correlation[4]={0};
			CvMat * matEnergy[4];
			CvMat* matEntropy[4];
			CvScalar tempEnergy[4];
			for(dim=0;dim<4;dim++)
			{
				matEnergy[dim] = cvCreateMat(16,16,CV_64FC1);
				cvMul(CoMatri[dim],CoMatri[dim],matEnergy[dim]) ;
				tempEnergy[dim] = cvSum(matEnergy[dim]);
				energy[dim] = tempEnergy[dim].val[0];
			}


			for (dim=0;dim<4;dim++)
			{

				Pmax[dim]=0;
				for (i=0;i<16;i++)
				{
					for (j=0;j<16;j++)
					{
						double tempEntropy = cvGetReal2D(CoMatri[dim],i,j);
						if (tempEntropy != 0)
						{
							entropy[dim] += -tempEntropy*log(tempEntropy);//各个角度下共生矩阵的熵
						}
						angleMoment[dim]  += tempEntropy*tempEntropy;//角二阶矩UNI
						inertia[dim] += (double)(i-j)*(double)(i-j)*tempEntropy;//反差或者惯性矩
						localSmooth[dim] += tempEntropy/((double)(i-j)*(double)(i-j)+1);//局部平滑
						DIS[dim] += tempEntropy*abs(i-j);//相异性
						correlation[dim] += (i*j*tempEntropy); //相关的一部分计算
						dSum[i] +=tempEntropy;
						if (tempEntropy>Pmax[dim])//最大概率
						{
							Pmax[dim] = tempEntropy;
						}

					}

				}

				for (int k=0;k<(16*2-1);k++)//计算和熵
				{
					SumIndexVal=0;//注意累加

					for(int g=0;(k<16)&&(g<=k);g++)
				 {

					 double sumInd= cvGetReal2D(CoMatri[dim],g,k-g);
					 if (sumInd>1e-15)
					 {
						 SumIndexVal +=(-1.0)* sumInd*log(sumInd);
					 }

				 }

					IndexVal[k] = SumIndexVal; //保存以x+y=k为index的元素值，计算和熵中涉及的参量

				}
				SumEntropy[dim]=SumIndexVal;//各个角度下的和熵



				for(int k=0;k<(16*2-1);k++ )	//计算sum variance
				{
					sumVar[dim] += (k-SumEntropy[dim])*(k-SumEntropy[dim])*(IndexVal[k]);//和熵的方差

				}


				for(int k=0;k<16;k++)//计算差熵
			 {
				 DifIndexVal = 0;
				 for (int g=0;g<16;g++)
				 {
					 //两个if语句用以实现abs(i-j)=k
					 if ((g-k)>=0&&(g-k)<16)
					 {
						 double difIndVal = cvGetReal2D(CoMatri[dim],g,g-k);
						 if (difIndVal>1e-15)
						 {
							 DifIndexVal += (-1.0)*difIndVal*log(difIndVal);
						 }

					 }

					 if ((g+k)>=0&&(g+k)<16)
					 {
						 double difIndVal = cvGetReal2D(CoMatri[dim],g,g+k);
						 if (difIndVal>1e-15)
						 {
							 DifIndexVal += (-1.0)*difIndVal*log(difIndVal);
						 }

					 }

				 }
				 IndexValDif[k] = DifIndexVal;//保存以|x-y|=k为index的元素值，计算差熵中涉及的参量
			 }

				DifEntropy[dim] += DifIndexVal ;// 差熵


				for (int k=0;k<16;k++)// 计算差熵的方差
				{
					difVar[dim]  += (k-DifEntropy[dim])*(k-DifEntropy[dim])*IndexValDif[k];
				}


				for (int x=0;x<16;x++)//计算相关中的均值
				{
					dMean += (x *dSum[x]);//求均值的两种方法！！！！
				}

				for (int x=0;x<16;x++)//计算相关中的方差
				{
					dStdDev += (x - dMean)*(x-dMean)*dSum[x];//求方差的两种方法！！！！
				}


				if(abs(dStdDev)>1e-15)//相关性
				{

					correlation[dim] += (correlation[dim] - dMean*dMean)/dStdDev;
				}
				else
				{
					correlation[dim]=0;

				}
			}

			//-------------建立一个包含四个角度共生矩阵的五个纹理参数的大矩阵---------------
			CvMat *paraMat[4];//共生矩阵四个角度
			for (dim=0;dim<4;dim++)//
			{

				paraMat[dim] = cvCreateMat(1,8,CV_64FC1);//每行里面存放某个角度下对应的5个纹理参数，这个行矩阵时高位1，宽为5，
				//所以下面的setReal2D时，注意高度始终为1，对应到其中的参数cvSetReal2D(paraMat[dim],0,0,tempVal);
				//第一个始终为0，表示列高，从0开始计算的
				cvZero(paraMat[dim]);//初始化矩阵

				double tempVal = energy[dim];//将能量参数的四个值

				cvSetReal2D(paraMat[dim],0,0,tempVal);

				tempVal = entropy[dim];

				cvSetReal2D(paraMat[dim],0,1,tempVal);

				tempVal = angleMoment[dim];

				cvSetReal2D(paraMat[dim],0,2,tempVal);

				tempVal = inertia[dim];
				cvSetReal2D(paraMat[dim],0,3,tempVal);

				tempVal = localSmooth[dim];
				cvSetReal2D(paraMat[dim],0,4,tempVal);

				tempVal = correlation[dim];
				cvSetReal2D(paraMat[dim],0,5,tempVal);

				tempVal = Pmax[dim];
				cvSetReal2D(paraMat[dim],0,6,tempVal);

				tempVal = DIS[dim];
				cvSetReal2D(paraMat[dim],0,7,tempVal);

			}

			//计算最终参数，即各个参数在不同角度上的均值和方差
			//static double *textureArray=NULL;
			GetTexturePara(paraMat,texturePara);
			/*	return (double*)textureArray;*/
			return 0;

}


double ComputeTextureDiff(IplImage *pImg1,IplImage *pImg2)
{
	double dResult = 0.0;
	//--------------图像转换为矩阵做运算-------------------------
	IplImage* pGrayImg1= cvCreateImage(cvGetSize(pImg1),pImg1->depth,1);//用于存储灰度图像
	IplImage* pGrayImg2= cvCreateImage(cvGetSize(pImg2),pImg2->depth,1);


	CvMat* pGrayMat1 = NULL; //图像转换为矩阵进行运算
	pGrayMat1 = cvCreateMat(pGrayImg1->height,pGrayImg1->width,CV_64FC1);
	CvMat* pGrayMat2 = NULL; //图像转换为矩阵进行运算
	pGrayMat2 = cvCreateMat(pGrayImg2->height,pGrayImg2->width,CV_64FC1);


	cvCvtColor(pImg1,pGrayImg1,CV_BGR2GRAY); //将彩色图像转换为灰度图像
	cvConvert(pGrayImg1,pGrayMat1); //将彩色图像转换为灰度图像,注意必须是pGrayImg进行图像到Mat的转换

	cvCvtColor(pImg2,pGrayImg2,CV_BGR2GRAY); //将彩色图像转换为灰度图像
	cvConvert(pGrayImg2,pGrayMat2); //将彩色图像转换为灰度图像,注意必须是pGrayImg进行图像到Mat的转换


	//compute:
	double texturePara1[16],texturePara2[16];
	/*double *ptexturePara1,*ptexturePara2;*/
	GetGLCM(pGrayMat1,texturePara1);
	GetGLCM(pGrayMat2,texturePara2);
	dResult = comparePara(texturePara1,texturePara2);


	return dResult;
}
