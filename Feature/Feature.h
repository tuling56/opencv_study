#include <opencv2/opencv.hpp>
using namespace cv;

//-----------------------------------【角点检测】------------------------------
int fast_corner_detect(char*filename);
int fast_feature_opencv(char*filename);
int harris_corner_detect(char*filename);
int harris_corner_detect_improve(char*filename);
int sift_feature();
int susan_corner_detect(char*filename);
int surf_feature();
int surf_feature_opencv();//该程序演示了模板匹配
void CheckFireAngle(Mat &img);//火灾尖角检测

//----------------------------------【Hog特征】-------------------------------

int hog_feature(); //未调通

//-----------------------------【garbor特征】-----------------------------
int garbor(char*filename);




