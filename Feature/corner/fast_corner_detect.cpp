#include <opencv2/opencv.hpp>

using namespace cv;

int fast_corner_detect(char*filename)
{
	cv::Mat  grayimage, srcimage = cv::imread (filename);  
	
	cv::cvtColor (srcimage,grayimage,CV_BGR2GRAY);  
	//¿ìËÙ½Çµã¼ì²â  
	std::vector<cv::KeyPoint> keypoints;  
	cv::FastFeatureDetector fast(40,true);  
	fast.detect (grayimage,keypoints);

	cv::drawKeypoints (grayimage,keypoints,srcimage,cv::Scalar::all(255),cv::DrawMatchesFlags::DRAW_OVER_OUTIMG); 
	
	//imwrite("fast_corner.jpg",srcimage);
	
	namedWindow("FastCornerDete",1);
	imshow("FastCornerDete",srcimage);

	return 0;
}
