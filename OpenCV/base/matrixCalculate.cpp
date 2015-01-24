/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	matrixCalculate.cpp
* Brief: Mat矩阵的相关统计特性的计算,均值，方差，协方差，最大最小值等
* Source:
* Status: 
* Date:	[1/21/2015 jmy]
************************************************************************/


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
 *	功能：求Mat的均值和标准差
 *  参数：
 *  状态：
 */
int matrixmeanStdDev(Mat&src)
{
	Mat mean, stddev;
	//CvScalar mean, stddev;//该结构在1.0版本中使用
	meanStdDev(src, mean, stddev);
	std::cout << "mean：" << std::endl << mean << std::endl << "stddev:" << std::endl << stddev << std::endl;
	waitKey();
	return 0;
}

/*
 *	功能：计算Mat的协方差矩阵
 *  参数：Mat的每个element代表一个向量，最简单的便是（B,G,R）三色通道的值
 *  状态：协方差矩阵的计算有错误，和手算的不一样
 */
int matrixcalcCovarMatrix(Mat&src)
{
	Mat_<float> samples[3]; //这里提供3个样本，每个样本的维度是4
	for (int i = 0; i < 3;i++)
	{
		samples[i].create(1, 4);
		samples[i](0, 0) = i * 4 + 1;
		samples[i](0, 1) = i * 4 + 2;
		samples[i](0, 2) = i * 4 + 3;
		samples[i](0, 3) = i * 4 + 4;
		cout <<" samples[i]:"<<endl<<samples[i] << endl;
	}
	
	Mat_<double> covMat; //计算得到的协方差方阵的维度和样本的维度相等，表征的是样本的每个特征向量之间的相关性
	Mat_<double> meanMat;
	calcCovarMatrix(samples,3,covMat, meanMat, CV_COVAR_NORMAL);
	//calcCovarMatrix(&samples, 1, covMat, meanMat, CV_COVAR_NORMAL|CV_COVAR_ROWS);

	string fileName = "covarMatrix.txt";
	FileStorage fs(fileName, FileStorage::WRITE);
	fs << "meanMat" << meanMat;
	fs << "covMat" << covMat;
	fs.release();

	return 0;
}


/*
 *	功能：计算Mat的mean和sum
 *  参数：
 *  状态：
 */
int matrixMeanSum(Mat&src)
{
	Scalar meanv = mean(src);//返回每个通道的均值
	Scalar sumv = sum(src); //返回每个通道的和
	std::cout <<"meanv: "<< meanv << std::endl;
	std::cout <<"sumv: "<< sumv << std::endl;
	
	return 0;
}

/*
 *	功能：计算Mat的距离和范数
 *  参数：
 *  状态：
 */
int matrixNorm(Mat &src)
{
	Mat mzeros = Mat::zeros(5, 5, CV_8U);
	Mat mones = Mat::ones(5, 5, CV_8U);
	double maxv=norm(mones, NORM_L2);
	cout << maxv << endl;
	

	return 0;
}


//功能测试区
int main()
{
	Mat src = imread("samples/rgb.png", 1);
	//matrixmeanStdDev(src);
	//matrixMeanSum(src);
	//matrixNorm(src);
	matrixcalcCovarMatrix(src);

	std::cin.get();
	return 0;
}
