#include<iostream>  
#include<opencv2/opencv.hpp>  

using namespace cv;
using namespace std;

int recto()
{
	Rect rect(0, 0, 50, 50);
	Point point(20, 20);
	Rect rect1(2, 2, 10, 10);
	Rect rect2(20, 20, 100, 100);
	Size size(5, 5);

	cout << "开始了" << endl;
	cout << "矩形左上角的横坐标：" << rect.x << endl;
	cout << "矩形左上角的纵坐标：" << rect.y << endl;
	cout << "矩形的宽度：" << rect.width << endl;
	cout << "矩形的高度：" << rect.height << endl;
	cout << "矩形的尺寸：" << rect.size() << endl;
	cout << "矩形的面积：" << rect.area() << endl;
	cout << "判断(20, 20)这个点在不在矩形内：" << rect.contains(point) << endl;
	/*cout << "判断矩形dstrect这个矩形在不在矩形内：" << rect.inside(rect1) << endl;*/
	cout << "矩形左上角的点坐标：" << rect.tl() << endl;
	cout << "矩形右下角的点坐标：" << rect.br() << endl;

	//矩阵的交集  
	cout << "矩阵的交集：" << (rect1 & rect) << endl;
	//矩阵的并集  
	cout << "矩阵的并集：" << (rect | rect2) << endl;

	//矩阵进行平移操作  
	cout << "矩阵进行平移操作：" << (rect + point) << endl;
	cout << "矩阵进行平移操作：" << (rect - point) << endl;
	//矩阵进行缩放操作  
	cout << "矩阵进行缩放操作：" << (rect + size) << endl;

	system("pause");

	return 0;
}

int main()
{
	recto();
}