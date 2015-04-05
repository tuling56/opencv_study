#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <iostream>

using namespace cv;

Point2f center(0,0);  

Point2f computeIntersect(Vec4i a,Vec4i b)  
{  
	int x1 = a[0],y1 = a[1],x2 = a[2],y2 = a[3],x3 = b[0],y3 = b[1],x4 = b[2],y4 = b[3];  

	if (float d = ((float)(x1 - x2)*(y3 - y4)-(y1 - y2)*(x3 - x4)))  
	{  
		Point2f pt;  
		pt.x = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4))/d;  
		pt.y = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4))/d;  
		return pt;  
	}  
	else  
		return Point2f(-1,-1);  
}  

void sortCorners(vector<Point2f>& corners,Point2f center)  
{  
	vector<Point2f> top,bot;  

	for (unsigned int i =0;i< corners.size();i++)  
	{  
		if (corners[i].y<center.y)  
		{  
			top.push_back(corners[i]);  
		}  
		else  
		{  
			bot.push_back(corners[i]);  
		}  
	}  

	Point2f tl = top[0].x > top[1].x ? top[1] : top[0];  
	Point2f tr = top[0].x > top[1].x ? top[0] : top[1];  
	Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];  
	Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];  

	corners.clear();  
	//注意以下存放顺序是顺时针，当时这里出错了，如果想任意顺序下文开辟的四边形矩阵注意对应  
	corners.push_back(tl);  
	corners.push_back(tr);  
	corners.push_back(br);  
	corners.push_back(bl);  

}  