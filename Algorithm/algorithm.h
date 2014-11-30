#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/*meanshift用于图像分割*/
int MeanShiftSegment();



/*HOG和SVM*/
int hog_svm_train();
int hog_svm_test();


/*KNN分类算法*/
int KNN();
int KNN1( int argc, char** argv );

/*模板匹配*/
int templateMatch();

/*目标检测*/
int find_obj();
int find_obj_calonder( int argc, char **argv );
int SURF_Homography();
