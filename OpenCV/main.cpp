#include "opencv.h"

int main(int argc,char*argv[])
{
	//读取文件夹内的指定格式的所有文件
	//char filename[50];
	//for (int i=1;i<6;i++)
	//{
	//	sprintf(filename,"PicVideo/videoimg/video_%d_cr.jpg",i);
	//	printf("正在处理%s\n",filename);
	//	//要进行的处理
	//	color_cluster(filename,2);
	//	cvWaitKey(0);
	//}
	
	
	char*videoname="samples//test_o2.mp4";
	char*imagename="samples//colorreduce.jpg";
		
	/*基础 */
    //rgb2hsv(imagename); //颜色空间转换
	//CalcVariance() ;//计算协方差矩阵
	//ReadSaveRGB();//读取图像的像素值并保存在txt文件中
    //visit_elem_Iplimage(imagename);
	//BrightContrastAdjust(imagename);//亮度和对比度调节

	
	
	/*视频读写 */
    //read_write_video(videoname,"out.wmv");
	//images2video();
	//read_video_info(argc,argv);

	
	/*人脸识别 */
	//face_detection(imagename);
	
	/*行人识别 */
	//walker_detection(imagename);
	
	/*灰度图像二值化*/
	//mythreshold(imagename);
	
	
	/*设置图像ROI*/
	//ROIimg(imagename);
	//roi_mat(); 
	//roi_merge();
	
	/*颜色聚类,颜色约减*/
	color_cluster(imagename,3);
		
	/*通道分离和合并*/
	//channel_split_merge();

	/*xml读写*/
	//xml_read_write();
	//Localtion areainfo={12,12,23,23};
	//opencv_xml_write(imagename,areainfo);

	/*访问和操作像素的值*/
	//visit_elem_Iplimage(imagename);
	//visit_elem_Mat(imagename);

	getchar();

	return 0;

  
}