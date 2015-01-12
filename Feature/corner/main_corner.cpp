#include "corner.h"
#include <iostream>

using namespace cv;
using namespace std;

int corner()
{
	char*filename="samples/firecorner.jpg";
	Mat img=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
	
	//surf_feature();
	//sift_feature();
	//surf_feature_opencv();

	//harris_corner_detect(filename);
	//susan_corner_detect(filename);
	//fast_corner_detect(filename);

	//------------------------------¡¾»ðÔÖ½Çµã¼ì²â¡¿-----------------------------
	//threshold(img,img,50,200,THRESH_BINARY);
	//CheckFireAngle(img);
	return 0;
}