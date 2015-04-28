/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	blob.cpp
* Brief: blobÌØÕ÷¿é¼ì²â
* Source:http://blog.csdn.net/xiaowei_cqu/article/details/8652096
* Status: 
* Date:	[4/6/2015 jmy]
************************************************************************/
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int blob()
{
	Mat image = imread("samples\\200.png");
	//Mat descriptors;
	vector<KeyPoint> keypoints;
	SimpleBlobDetector::Params params;
	params.minThreshold = 10;
	params.maxThreshold = 100;
	params.thresholdStep = 10;
	params.minDistBetweenBlobs = 100;
	
	params.filterByColor = false;
	params.blobColor = 255;

	params.filterByConvexity;
	params.minConvexity = 0.3;
	params.maxConvexity = 10;

	params.filterByInertia = false;
	params.minInertiaRatio = 0.01;
	
	params.filterByArea = true;
	params.minArea = 100;
	params.maxArea = 8000;
	
	params.filterByCircularity = false;


	SimpleBlobDetector blobDetector(params);
	blobDetector.create("SimpleBlob");
	blobDetector.detect(image, keypoints);
	drawKeypoints(image, keypoints, image, Scalar(255, 0, 0));

	imshow("image", image);
	waitKey();
	//SimpleBlobDetector.findBlobs()
	
	return 0;

}