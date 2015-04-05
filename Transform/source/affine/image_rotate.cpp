/*
 *	插值算法的实现图像旋转
 */

#include<cv.h>
#include <highgui.h>


//最近邻插值法实现图像旋转45度
int rotate_nearest_neibgour(char*filename)
{
	cv::Mat matSrc = cv::imread(filename, 2 | 4);

	if (matSrc.empty()) return -1;

	const double degree = 45;
	double angle = degree * CV_PI / 180.;
	double alpha = cos(angle);
	double beta = sin(angle);
	int iWidth = matSrc.cols;
	int iHeight = matSrc.rows;
	int iNewWidth = cvRound(iWidth * fabs(alpha) + iHeight * fabs(beta));
	int iNewHeight = cvRound(iHeight * fabs(alpha) + iWidth * fabs(beta));

	double m[6];
	m[0] = alpha;
	m[1] = beta;
	m[2] = (1 - alpha) * iWidth / 2. - beta * iHeight / 2.;
	m[3] = -m[1];
	m[4] = m[0];
	m[5] = beta * iWidth / 2. + (1 - alpha) * iHeight / 2.;

	cv::Mat M = cv::Mat(2, 3, CV_64F, m);
	cv::Mat matDst1 = cv::Mat(cv::Size(iNewWidth, iNewHeight), matSrc.type(), cv::Scalar::all(0));

	double D = m[0]*m[4] - m[1]*m[3];
	D = D != 0 ? 1./D : 0;
	double A11 = m[4]*D, A22 = m[0]*D;
	m[0] = A11; m[1] *= -D;
	m[3] *= -D; m[4] = A22;
	double b1 = -m[0]*m[2] - m[1]*m[5];
	double b2 = -m[3]*m[2] - m[4]*m[5];
	m[2] = b1; m[5] = b2;

	int round_delta = 512;//nearest
	for (int y=0; y<iNewHeight; ++y)
	{
		for (int x=0; x<iNewWidth; ++x)
		{
			//int tmpx = cvFloor(m[0] * x + m[1] * y + m[2]);
			//int tmpy = cvFloor(m[3] * x + m[4] * y + m[5]);
			int adelta = cv::saturate_cast<int>(m[0] * x * 1024);
			int bdelta = cv::saturate_cast<int>(m[3] * x * 1024);
			int X0 = cv::saturate_cast<int>((m[1] * y + m[2]) * 1024) + round_delta;
			int Y0 = cv::saturate_cast<int>((m[4] * y + m[5]) * 1024) + round_delta;
			int X = (X0 + adelta) >> 10;
			int Y = (Y0 + bdelta) >> 10;

			if ((unsigned)X < iWidth && (unsigned)Y < iHeight)
			{
				matDst1.at<cv::Vec3b>(y, x) = matSrc.at<cv::Vec3b>(Y, X);
			}
		}
	}
	cv::imwrite("rotate_nearest_1.jpg", matDst1);

	M = cv::getRotationMatrix2D(cv::Point2f(iWidth / 2., iHeight / 2.), degree, 1);

	cv::Mat matDst2;
	cv::warpAffine(matSrc, matDst2, M, cv::Size(iNewWidth, iNewHeight), 0, 0, 0);
	cv::imwrite("rotate_nearest_2.jpg", matDst2);
	return 0;
}