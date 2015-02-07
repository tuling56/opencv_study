//-----------------------------------【图像平滑】------------------------------
//	描述:低通滤波
//		  ---blur
//		  ---GaussianBlur
//		  ---medianBlur
//  参考：http://blog.csdn.net/xiaowei_cqu/article/details/7785365
//  版本:opencv2.4.2
//  说明：双边滤波的参数不好条件，几乎看不到滤波效果
//-------------------------------------------------------------------------

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int opencv2_filter(char*path)
{
    using namespace cv;

    Mat image=imread(path);
    //cvtColor(image,image,CV_BGR2GRAY);//opencv2.0中通道转换函数

    Mat blurResult;
    Mat gaussianResult;
    Mat medianResult;
	Mat bilateraResult;

    blur(image,blurResult,Size(5,5));  //简单模糊，OpenCV中的 blur函数是进行标准方框滤波：
    GaussianBlur(image,gaussianResult,Size(5,5),1.5); //高斯滤波
    medianBlur(image,medianResult,5); //中值滤波
	bilateralFilter(image, bilateraResult, 3, 20, 20,0);//双边滤波

    namedWindow("blur");imshow("blur",blurResult);
    namedWindow("Gaussianblur");imshow("Gaussianblur",gaussianResult);
    namedWindow("medianBlur");imshow("medianBlur",medianResult);
	namedWindow("bilateralBlur");imshow("bilateralBlur",medianResult);

    waitKey();
    return 0;
}

//功能测试区
//int main(int argc,char**argv)
//{
// 
//	opencv2_filter("samples\\img_208.jpg");
//    getchar();
//	return 0;
//}