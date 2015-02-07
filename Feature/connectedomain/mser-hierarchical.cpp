/* A simple demonstration of Hierarchical MSER.

   Hierarchical MSER is introduced in this paper:

      Carlos Merino-Gracia, Karel Lenc and Majid Mirmehdi. 
      A Head-mounted Device for Recognizing Text in Natural Scenes.
      In CBDAR 2011.
      (http://www.cs.bris.ac.uk/Publications/Papers/2001440.pdf)
*/
/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	mser-hierarchical.cpp
* Brief: 
* Source:
* Status: 未调通
* Date:	[1/30/2015 jmy]
************************************************************************/

#include <vector>
#include <list>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//遍历文件函数声明
void getFiles(string path, vector<string>& files);

void drawMSER(cv::Mat &image, CvSeq *seq, int level = 0) 
{
   cv::Vec3b color_vec;
   int color_val = 50 + level * 205 / 5;
   if (((CvContour *)seq)->color >= 0)
      color_vec = cv::Vec3b(0,0, color_val);
   else
      color_vec = cv::Vec3b(color_val,0,0);

   for (int j = 0; j < seq->total; ++j) {
      CvPoint *pos = CV_GET_SEQ_ELEM(CvPoint, seq, j);
      image.at<cv::Vec3b>(pos->y, pos->x) = color_vec;
   }

   // Walk all the children of this node.
   CvSeq *it = seq->v_next;
   while (it) {
      drawMSER(image, it, level + 1);
      it = it->h_next;
   }
}

void mserTest(const Mat &image, Mat &draw_image) 
{
   cv::Ptr<CvMemStorage> storage(cvCreateMemStorage(0));
   CvSeq *contours;
   cvExtractMSER(&(IplImage)image, NULL, &contours, storage, cvMSERParams());

   for (size_t i = 0; i < contours->total; ++i) {
      CvSeq *seq = *CV_GET_SEQ_ELEM(CvSeq *, contours, i);
      // No parent, so it is a root node.
      if (seq->v_prev == NULL)
         drawMSER(draw_image, seq);
   }

   cout << "Number of MSERs: " << contours->total << endl;
}


//功能测试区
//int main(int argc, char *argv[]) 
//{
//	string path = "samples\\msra\\";
//	vector<string> filenames;
//	getFiles(path, filenames);
//
//	for (vector<string>::iterator it = filenames.begin(); it < filenames.end(); it++)
//	{
//		Mat src = imread(*it, 1);
//		Mat gray;
//		cvtColor(src, gray, CV_BGR2GRAY);
//		mserTest(gray, src);
//		imshow("Color mser", src);
//		
//		//imwrite("result.jpg", drawImage);
//		waitKey(0);
//	}
//
//	return 0;
//}

