/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	main.cpp
* Brief: 形态学操作的主入口
* Source:
* Status: 
* Date:	[3/7/2015 jmy]
************************************************************************/
#include "morphology.h"

using namespace  std;
using namespace cv;

int main()
{
	string filename="samples\\test2.jpg";
	Mat src = imread(filename, 0);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	//morphologyShow();
	//morphy_whole(filename);
	Mat opened;
	morphologyEx(src, opened, MORPH_OPEN, element);
	imwrite("opend.jpg", opened);

	Mat closed;
	morphologyEx(src, closed, MORPH_CLOSE, element);
	imwrite("closed.jpg", closed);



	//thin(filename);
	return 0;

}