#include <opencv2/opencv.hpp>
//-----------------------------------¡¾½Çµã¼ì²â¡¿-----------------------------
//ÃèÊö£º½Çµã¼ì²â
//-----------------------------------------------------------------------
int fast_corner_detect(char*filename);
int fast_feature_opencv(char*filename);
int harris_corner_detect(char*filename);
int harris_corner_detect_improve(char*filename);
int sift_feature();
int susan_corner_detect(char*filename);
int surf_feature();
int surf_feature_opencv();//¸Ã³ÌĞòÑİÊ¾ÁËÄ£°åÆ¥Åä
void CheckFireAngle(cv::Mat &img);//»ğÔÖ¼â½Ç¼ì²â