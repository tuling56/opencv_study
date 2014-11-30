#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

/*******数据*********/
struct Localtion{  
	unsigned int lx;  
	unsigned int ly;  
	unsigned int rx;  
	unsigned int ry;    
};

//基础
int rgb2hsv(char*filename); //颜色空间转换
int CalcVariance() ;//计算协方差矩阵
int ReadSaveRGB();
int BrightContrastAdjust(char*path );//亮度和对比度调节


//视频读取和写入
int read_write_video(char*videoin,char*videoout); 
int images2video();  
int read_video_info(int para_num, char* paras[]);

//访问和操作像素的值
void visit_elem_Iplimage(char*picname);
void visit_elem_Mat(char*picname);

//人脸检测
int face_detection(char*filename);
//行人检测
int walker_detection(char*filename);

//灰度图像二值化
int mythreshold(char*filename);


//设置图像ROI
int ROIimg(char*filename);
int roi_mat();//该方法采用mat格式
int roi_merge();//设置roi和图像融合

//颜色聚类
int color_cluster(char *filename,int nCuster);

//通道分离和合并
void channel_split_merge();
 
//xml读写
int xml_read_write(void);
int opencv_xml_write(char* photoname,Localtion areainfo);

