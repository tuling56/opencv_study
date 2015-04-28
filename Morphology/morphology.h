/************************************************************************/
/* 函数头描述                                                    
/************************************************************************/
#include <string>
#include <opencv2/opencv.hpp>

int morphologyShow();//整体演示
int morphy_whole(std::string filename);//整体演示，包括顶帽和黑帽变换

int erode_dilate();
int open_close();
int morphy_edge();//边缘检测
int morphy_corner();//角点检测
int morphy_apply_location();//定位

//------------------------------------图像细化-----------------------------
int thin(char*input);

