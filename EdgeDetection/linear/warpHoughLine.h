#if !defined LINEF
#define LINEF
#include<cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define PI 3.1415926
using namespace cv;
using namespace std;
class LineFinder {

private:

	// original image
	Mat img;

	// vector containing the end points 
	// of the detected lines
	vector<Vec4i> lines;

	// accumulator resolution parameters
	double deltaRho;
	double deltaTheta;

	// minimum number of votes that a line 
	// must receive before being considered
	int minVote;

	// min length for a line
	double minLength;

	// max allowed gap along the line
	double maxGap;

public:

	// Default accumulator resolution is 1 pixel by 1 degree
	// no gap, no mimimum length
	LineFinder() : deltaRho(1), deltaTheta(PI / 180), minVote(10), minLength(0.), maxGap(0.) {}

	// Set the resolution of the accumulator
	void setAccResolution(double dRho, double dTheta) {

		deltaRho = dRho;
		deltaTheta = dTheta;
	}

	// Set the minimum number of votes
	void setMinVote(int minv) {

		minVote = minv;
	}

	// Set line length and gap
	void setLineLengthAndGap(double length, double gap) {

		minLength = length;
		maxGap = gap;
	}

	// Apply probabilistic Hough Transform
	vector<Vec4i> findLines(Mat& binary) {

		lines.clear();
		HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);

		return lines;
	}

	// Draw the detected lines on an image
	void drawDetectedLines(Mat &image, Scalar color = Scalar(255, 0, 0)) {

		// Draw the lines
		vector<Vec4i>::const_iterator it2 = lines.begin();

		while (it2 != lines.end()) {

			Point pt1((*it2)[0], (*it2)[1]);
			Point pt2((*it2)[2], (*it2)[3]);
			double slope = fabs(double((*it2)[1] - (*it2)[3]) / ((*it2)[0] - (*it2)[2]));
			// double slope = fabs (((double)(lines[1].y-lines[0].y))/((double)(lines[1].x-lines[0].x)));
			//求直线在坐标系中的斜率
			//double length=sqrt((line[1].y-line[0].y)*(line[1].y-line[0].y)+(line[1].x-line[0].x)*(line[1].x-line[0].x));
			////线段的长度
			//if((slope>0.3)&&(slope<1000)&&(length>50)&&(length<500))
			//{				
			//	line[0].y= line[0].y+ROI_rect_src.y;
			//	line[1].y =line[1].y+ROI_rect_src.y;
			//	cvLine(frame, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
			//}
			if ((slope > 0.52) && (slope < 2))
			{
				line(image, pt1, pt2, color, 3, 8, 0);
			}
			++it2;
		}
	}


};


#endif