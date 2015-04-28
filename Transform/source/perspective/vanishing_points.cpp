#include<opencv2/opencv.hpp>
using namespace cv;
// Vanishing point detection sample based on
// http://dasl.mem.drexel.edu/wiki/index.php/Vanishing_point_detection_for_corridors_and_hallways

// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------
int main(int argc, char** argv)
{

	// Thresholds
	int th1 = 0, th2 = 255;

	// Create a window
	cvNamedWindow("canny");
	cvCreateTrackbar("th1", "canny", &th1, 255);
	cvCreateTrackbar("th2", "canny", &th2, 255);

	// Get an image
	IplImage *image = cvLoadImage("samples\\ts.png");
	IplImage *gray_out = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	IplImage *canny_out = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);

	// Convert the image to grayscale
	cvCvtColor(image, gray_out, CV_RGB2GRAY);

	// Denoising
	cvSmooth(gray_out, gray_out, CV_GAUSSIAN, 3, 3);

	// Detect edges
	cvCanny(gray_out, canny_out, th1, th2, 3);

	// Detect lines
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq *lines = cvHoughLines2(canny_out, storage, CV_HOUGH_STANDARD, 1, CV_PI / 180, 100);

	// Hough standard transform
	std::vector<float> m1, c1, xvc, yvc;
	for (int i = 0; i < MIN(lines->total, 100); i++)
	{
		float *line = (float*)cvGetSeqElem(lines, i);
		float rho = line[0];
		float theta = line[1];
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;

		// Line points 
		CvPoint pt1, pt2;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		// Save angled line
		if (fabs(theta) > 0.0)
		{
			float m = -cos(theta) / sin(theta);
			float c = rho * (1.0 / sin(theta));
			if (0.5 < fabs(m) && fabs(m) < 5.0)
			{
				m1.push_back(m);
				c1.push_back(c);
				cvLine(image, pt1, pt2, CV_RGB(255, 0, 0));
			}
		}
	}

	// Detect vanishing points
	for (int k = 0; k < (int)m1.size(); k++) 
	{
		for (int k1 = k + 1; k1 < (int)m1.size(); k1++) 
		{
			if (fabs(m1[k] - m1[k1]) > 1.0) 
			{
				int xv = fabs((c1[k1] - c1[k]) / (m1[k] - m1[k1]));
				int yv = fabs((m1[k1] * xv) + c1[k1]);
				if (yv > 0 && yv < image->height && xv > 0 && xv < image->width) 
				{
					xvc.push_back(xv);
					yvc.push_back(yv);
				}
			}
		}
	}

	// Detected
	if (!xvc.empty()) 
	{
		double sum_x = 0, sum_y = 0, sum_w = 0;
		for (int i = 0; i < (int)xvc.size(); i++) 
		{
			double w = exp(-hypot(xvc[i] - canny_out->width / 2, yvc[i] - canny_out->height / 2));
			sum_x += w * xvc[i];
			sum_y += w * yvc[i];
			sum_w += w;
		}

		// Vanishing point
		CvPoint vanishing_point = cvPoint(sum_x / sum_w, sum_y / sum_w);

		// Show result
		cvCircle(image, vanishing_point, 5, cvScalar(255, 0, 0), 2);
	}

	// Display the image
	cvShowImage("canny", canny_out);
	cvShowImage("camera", image);

	cvWaitKey();

	// Release memories
	cvReleaseImage(&gray_out);
	cvReleaseImage(&canny_out);
	cvReleaseMemStorage(&storage);

	

	return 0;
}