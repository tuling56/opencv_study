//参考：http://blog.csdn.net/xiaowei_cqu/article/details/23782561
#include <iostream>
#include <time.h>  //time(0)的头文件
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp> //CvKNearest在这个头文件中


using namespace cv;
using namespace std;

int KNN()  
{  
	float labels[10] = {0,0,0,0,0,1,1,1,1,1};  //两类 
	Mat labelsMat(10, 1, CV_32FC1, labels);  
	cout<<labelsMat<<endl;  

	float trainingData[10][2];  
	srand(time(0));   
	for(int i=0;i<5;i++){  
		trainingData[i][0] = rand()%255+1;  
		trainingData[i][1] = rand()%255+1;  
		trainingData[i+5][0] = rand()%255+255;  
		trainingData[i+5][1] = rand()%255+255;  
	}  
	Mat trainingDataMat(10, 2, CV_32FC1, trainingData);  
	cout<<trainingDataMat<<endl;  

	CvKNearest knn;  
	knn.train(trainingDataMat,labelsMat,Mat(), false, 2 );  
	
	// Data for visual representation  
	int width = 512, height = 512;  
	Mat image = Mat::zeros(height, width, CV_8UC3);  
	Vec3b green(0,255,0), blue (255,0,0);  

	for (int i = 0; i < image.rows; ++i)
	{  
		for (int j = 0; j < image.cols; ++j)
		{  
			const Mat sampleMat = (Mat_<float>(1,2) << i,j);  
			Mat response;  
			float result = knn.find_nearest(sampleMat,1);  
			if (result !=0){  
				image.at<Vec3b>(j, i)  = green;  
			}  
			else    
				image.at<Vec3b>(j, i)  = blue;  
		}  
	}  

	// Show the training data  
	for(int i=0;i<5;i++)
	{  
		circle( image, Point(trainingData[i][0],trainingData[i][1]),5, Scalar(  0,   0,   0), -1, 8);  
		circle( image, Point(trainingData[i+5][0],trainingData[i+5][1]),5, Scalar(255, 255, 255), -1, 8);  
	}  

	imshow("KNN Simple Example", image); // show it to the user  
	waitKey(10000);  

	return 0;

}  