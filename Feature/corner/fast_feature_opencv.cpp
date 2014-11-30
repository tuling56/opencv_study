#include <opencv2/core/core.hpp>     
#include <opencv2/features2d/features2d.hpp>     
#include <opencv2/highgui/highgui.hpp>  
#include <vector>  

using namespace cv;  

int fast_feature_opencv(char*filename)  
{  
	Mat src,dst;  
	src = imread(filename);
	dst = imread("PicVideo//box.png");
	// vector of keyPoints  
	vector<KeyPoint> keyPoints0,keyPoints1;  
	// construction of the fast feature detector object  
	FastFeatureDetector fast(40);   // 检测的阈值为40  
	// feature point detection  
	fast.detect(src,keyPoints0);
	fast.detect(dst,keyPoints1);
	drawKeypoints(src, keyPoints0, src, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS); 
	drawKeypoints(dst, keyPoints1, dst, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS); 
	imshow("FAST feature", src); 
	imshow("Object",dst);
	//匹配部分还未完成
	/*drawMatches(src,keyPoints0,dst,keyPoints1,dst,
				Scalar::all(-1),Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	 */
	cvWaitKey(0);  
	return 0;
}  