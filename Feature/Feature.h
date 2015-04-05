#include <opencv2/opencv.hpp>
using namespace cv;


//------------------------------【功能函数】-----------------------------
void getFiles(string path, vector<string>& files);

//------------------------------【角点检测】------------------------------
#include <corner\corner.h>

//------------------------------【直方图】-----------------------------
#include <histgram\histgram.h>

//------------------------------【Hog特征】-------------------------------
int hog_feature(); //未调通

//-----------------------------【garbor特征】-----------------------------
int garbor(char*filename);





