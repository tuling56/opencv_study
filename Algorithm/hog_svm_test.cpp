#include "opencv2/opencv.hpp"
#include "windows.h"
#include "fstream"

using namespace std;
using namespace cv;


int hog_svm_test()
{

	CvSVM svm = CvSVM();
	svm.load("HOG_SVM_DATA.xml");//加载训练好的xml文件，这里训练的是10K个手写数字
	//检测样本    
	IplImage *test;  
	char result[300]; //存放预测结果 
	test = cvLoadImage("8.png", 1); //待预测图片，用系统自带的画图工具随便手写
	if (!test)
	{
		MessageBox(NULL,TEXT("待预测图像不存在！"),TEXT("提示"),MB_ICONWARNING);
		return -1;
	}
	IplImage* trainTempImg=cvCreateImage(cvSize(28,28),8,3);
	cvZero(trainTempImg);    
	cvResize(test,trainTempImg);     
	HOGDescriptor *hog=new HOGDescriptor(cvSize(28,28),cvSize(14,14),cvSize(7,7),cvSize(7,7),9);      
	vector<float>descriptors;//存放结果       
	hog->compute(trainTempImg, descriptors,Size(1,1), Size(0,0)); //Hog特征计算      
	cout<<"HOG dims: "<<descriptors.size()<<endl;  //打印Hog特征维数，这里是324
	CvMat* SVMtrainMat=cvCreateMat(1,descriptors.size(),CV_32FC1);   
	int n=0;    
	for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)    
	{    
		cvmSet(SVMtrainMat,0,n,*iter);    
		n++;    
	}   

	int ret = svm.predict(SVMtrainMat);//检测结果
	sprintf(result, "%d\r\n",ret );

	cvNamedWindow("dst",1);
	cvShowImage("dst",test);

	MessageBox(NULL,result,TEXT("预测结果"),MB_OK);

	cvReleaseImage(&test);
	cvReleaseImage(&trainTempImg);
	return 0;
}