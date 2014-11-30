//histgram功能的主入口

#include "histgram.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int histgram()
{
	char*filename="zimu.jpg";

	//-----------------------------------【直方图绘制】------------------------------
	//	描述:
	//-------------------------------------------------------------------------
	//calcDrawHist(filename);
	//oneDim(filename);
	mutiDim(filename);

	//-----------------------------------【直方图均衡化】------------------------------
	//	描述:
	//-------------------------------------------------------------------------
	//equalizeHist(filename);

	

	return 0;
}