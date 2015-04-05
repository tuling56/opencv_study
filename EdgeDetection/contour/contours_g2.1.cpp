//来自：http://www.cnblogs.com/wsc36305/archive/2012/10/15/2725218.html

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
	// Read input binary image
	 Mat image= imread("./binaryGroup.bmp",0);
	 if (!image.data)
		return 0; 
	 namedWindow("Binary Image");
	 imshow("Binary Image",image);

	// Get the contours of the connected components
	 vector<vector<Point>> contours;
	 //findContours的输入是二值图像
	 findContours(image, 
		contours, // a vector of contours 
		CV_RETR_EXTERNAL, // retrieve the external contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

	// Print contours' length轮廓的个数
	cout << "Contours: " << contours.size() << endl;
	vector<vector<Point>>::const_iterator itContours= contours.begin();
	for ( ; itContours!=contours.end(); ++itContours) {

		cout << "Size: " << itContours->size() << endl;//每个轮廓包含的点数
	}

	// draw black contours on white image
	 Mat result(image.size(),CV_8U,Scalar(0));
	 drawContours(result,contours,      //画出轮廓
		-1, // draw all contours
		Scalar(255), // in black
		2); // with a thickness of 2

	namedWindow("Contours");
	imshow("Contours",result);

	// Eliminate too short or too long contours
	int cmin= 100;  // minimum contour length
	int cmax= 1000; // maximum contour length
	vector<vector<Point>>::const_iterator itc= contours.begin();
	while (itc!=contours.end()) {

		if (itc->size() < cmin || itc->size() > cmax)
			itc= contours.erase(itc);
		else 
			++itc;
	}

	// draw contours on the original image
	Mat original= imread("./group.bmp");
	drawContours(original,contours,
		-1, // draw all contours
		Scalar(255,255,255), // in white
		2); // with a thickness of 2

	namedWindow("Contours on Animals");
	imshow("Contours on Animals",original);

	// Let's now draw black contours on white image
	//result.setTo(Scalar(0));
	//If the third parameter of this function is a negative value, then all contours are drawn.
	//Otherwise, it is possible to specify the index of the contour to be drawn
	drawContours(result,contours,
		-1, // draw all contours
		Scalar(255), // in black
		1); // with a thickness of 1
	image= imread("./binaryGroup.bmp",1);

	// testing the bounding box 
	Rect r0= boundingRect(Mat(contours[0]));//boundingRect获取这个外接矩形
	rectangle(result,r0,Scalar(255,255,255),2);

	// testing the enclosing circle 
	float radius;
	Point2f center;
	minEnclosingCircle(Mat(contours[1]),center,radius);//对轮廓进行多变形逼近
	circle(result,Point(center),static_cast<int>(radius),Scalar(255),2);

	RotatedRect rrect= fitEllipse(Mat(contours[2]));
	ellipse(result,rrect,Scalar(255),2);

	 //testing the approximate polygon
	vector<Point> poly;
	approxPolyDP(Mat(contours[2]),poly,5,true);

	cout << "Polygon size: " << poly.size() << endl;

	//// Iterate over each segment and draw it
	vector<Point>::const_iterator itp= poly.begin();
	while (itp!=(poly.end()-1)) {
		line(result,*itp,*(itp+1),Scalar(255),2);
		++itp;
	}
	// last point linked to first point
	line(result,*(poly.begin()),*(poly.end()-1),Scalar(20),2);

	// testing the convex hull
	vector<Point> hull;
	convexHull(Mat(contours[3]),hull);

	// Iterate over each segment and draw it
	vector<Point>::const_iterator it= hull.begin();
	while (it!=(hull.end()-1)) {
		line(result,*it,*(it+1),Scalar(255),2);
		++it;
	}
	// last point linked to first point
	line(result,*(hull.begin()),*(hull.end()-1),Scalar(255),2);

	// testing the moments

	//iterate over all contours
	itc= contours.begin();
	while (itc!=contours.end()) {

		// compute all moments
		Moments mom= moments(Mat(*itc++));

		// draw mass center
		circle(result,
			// position of mass center converted to integer
			Point(mom.m10/mom.m00,mom.m01/mom.m00),
			2,Scalar(255),2); // draw black dot
	}

	namedWindow("Some Shape descriptors");
	imshow("Some Shape descriptors",result);

	// New call to findContours but with CV_RETR_LIST flag
	image= imread("./binaryGroup.bmp",0);

	// Get the contours of the connected components
	findContours(image, 
		contours, // a vector of contours 
		CV_RETR_LIST, // retrieve the external and internal contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

	// draw black contours on white image
	result.setTo(Scalar(0));
	drawContours(result,contours,
		-1, // draw all contours
		Scalar(255), // in black
		2); // with a thickness of 2
	namedWindow("All Contours");
	imshow("All Contours",result);

	waitKey();
	return 0;
}