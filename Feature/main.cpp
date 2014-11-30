//特征提取函数的主入口
#include <iostream>
#include "Feature.h"

using namespace std;

int main()
{
	char*filename="samples/firecorner.jpg";
	Mat img=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
	//garbor(filename);
	//-----------------------------------【角点检测和匹配】-----------------------------
	//surf_feature();
	//sift_feature();
	//surf_feature_opencv();
	//-----------------------------------【角点检测】-----------------------------
	//harris_corner_detect(filename);
	//susan_corner_detect(filename);
	//fast_corner_detect(filename);
	threshold(img,img,50,200,THRESH_BINARY);
	CheckFireAngle(img);


	cout<<"====主程序结束========="<<endl;
	cin.get();
	return 0;
}