#include "harris_corner_detect.h"

using namespace cv;

int harris_corner_detect(char*filename)
{
	cv::Mat  image, image1 = cv::imread (filename);
	//灰度变换
	cv::cvtColor (image1,image,CV_BGR2GRAY);

	//经典的harris角点方法
	harris Harris;
	// 计算角点
	Harris.detect(image);
	//获得角点
	std::vector<cv::Point> pts;
	Harris.getCorners(pts,0.01);
	// 标记角点
	Harris.drawOnImage(image1,pts);

	cv::namedWindow ("harris");
	cv::imshow ("harris",image1);
	cv::waitKey (0);
	return 0;

}

//其实就是opencv中使用的harris角点检测
int harris_corner_detect_improve(char*filename)
{
	cv::Mat  grayimage, srcimage = cv::imread (filename);  
	//灰度变换  
	cv::cvtColor (srcimage,grayimage,CV_BGR2GRAY);  
	// 改进的harris角点检测方法  
	std::vector<cv::Point> corners;  
	cv::goodFeaturesToTrack(grayimage,corners,  
		200,  //角点最大数目  
		0.01,  // 质量等级，这里是0.01*max（min（e1，e2）），e1，e2是harris矩阵的特征值  
		1  // 两个角点之间的距离容忍度
		);  
	  
	harris().drawOnImage(srcimage,corners);//标记角点 

	imshow("harris_improve.jpg",srcimage);
	
	return 0;
}


