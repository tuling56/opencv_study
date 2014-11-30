#include <opencv2/opencv.hpp>
#include <iostream>

IplImage* NormalizeImage(IplImage *img,IplImage*out)

{

	//1、创建归一化的图像；

	    //IplImage* out = cvCreateImage(cvGetSize(img), 8, 3);

	//2、获取图像高度和宽度信息，设置epslon的目的是防止除0的操作产生；

		int width = img->width;
		int height = img->height;
		int redValue, greenValue, blueValue;
		double sum, epslon = 0.000001;

	//3、计算归一化的结果，并替换掉原像素值；

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				CvScalar src = {0};
					src=cvGet2D(img, y, x);
				redValue = src.val[0];
				greenValue = src.val[1];
				blueValue = src.val[2];
				// 加上epslon，为了防止除以0的情况发生
				sum = redValue + greenValue + blueValue + epslon;
				CvScalar des = cvScalar(redValue / sum * 255, greenValue / sum * 255, blueValue / sum * 255, src.val[4]);
				cvSet2D(out, y, x, des);

			}
		}     

	/*	4、返回归一化后的图像；*/

			return out;

}

int main()
{
	 const char* filename="(1)pp.png";
	 IplImage * Inputimage = cvLoadImage ( filename );
	 IplImage * Outputimage= cvCreateImage(cvGetSize(Inputimage), 8, 3);
	 NormalizeImage(Inputimage,Outputimage);
	 cvShowImage("NormailizeImage",Outputimage);
     getchar();
	 return 0;

}