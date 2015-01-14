/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	visit_elem_mat.cpp
* Brief: 访问Mat中每个像素的值（新）
* Source:http://blog.csdn.net/xiaowei_cqu/article/details/19839019
* Status: 窗口扫描部分完成，但速度太慢了
* Date:	[1/10/2015 jmy]
************************************************************************/

#include <opencv2/core/core.hpp>
#include <highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void visit_elem(string picname)
{
	int div=4;
	Mat img=imread(picname);
	namedWindow("origin",1);
    imshow("origin",img);

	/********方式一：at<typename>(i,j)*******/
	Mat_<uchar> im=img; //这里将一个像素的三个通道分开，这样原来的一列变成3列
	//Mat_<Vec3b> im=img;
	for (int i=0;i<img.rows;i++)
	{
		for (int j=0;j<img.cols;j++)
		{
			img.at<Vec3b>(i,j).val[0]=img.at<Vec3b>(i,j).val[0]/div*div+div/2;
			img.at<Vec3b>(i,j).val[1]=img.at<Vec3b>(i,j).val[1]/div*div+div/2;
			img.at<Vec3b>(i,j).val[2]=img.at<Vec3b>(i,j).val[2]/div*div+div/2;
			
			im(i,j)=im(i,j)/div*div+div/2;
		}
	}
	namedWindow("output",1);
	imshow("output",im);
	waitKey(0);
	
}

int sortm(const int &v1, const int &v2)
{
	return v1 > v2;
}

/*
 *	功能：窗扫描
 *  输入：
 *  输出：
 *  状态：
 */

int wscan(string picname)
{
	int w = 9;
	int num = 0;
	int mean = 0;
	vector<int> v1;
	Mat img = imread(picname,0);
	Mat result;
	img.copyTo(result);
	imshow("o", img);

	int myu,myd,nxl,nxr;//定义搜索界限
	double duration=static_cast<double>(getTickCount());
	for (int i = 0; i < img.rows; i++)
	{
		myu=(i - w / 2) > 0 ? (i - w / 2) : 0;
		myd=(i + w / 2)>img.rows ? img.rows:(i+w/2);
		for (int j = 0; j < img.cols; j++)
		{
			num++;
			cout << num << endl;
			//cout << "row " << i << " col: " << j << ": " << int(img.at<uchar>(i, j)) << endl;
			nxl=(j - w / 2) > 0 ? (j - w / 2) : 0;
			nxr=(j + w / 2)>img.cols ? img.cols : (j + w / 2);
			for (int m = myu; m<myd;m++)
			{
				//uchar*data=img.ptr<uchar>(m);
				for (int n = nxl; n<nxr; n++)
				{
					//v1.push_back(data[n]);
					v1.push_back(img.at<uchar>(m, n));
				}
			}
			//对vector中的元素进行排序，去除最大值，然后取前5个值的平均值
			std::sort(v1.begin(), v1.end(), sortm);
			for (int i = 1; i < 6;i++)	{
				mean = mean + v1[i];
			}
			mean = mean / 5;
			
			//对该点灰度进行重新赋值
			result.at<uchar>(i, j) = mean;
			
			//清空容器
			v1.clear();
		}
	}
	duration=static_cast<double>(getTickCount())-duration;
	duration/=getTickFrequency();
	duration=duration/1000;
	cout<<"耗时(秒)："<<duration<<endl;
	imwrite("wbg.png",result);
	imshow("c", result);
	waitKey(0);
	return 0;
}

//功能测试区
//int main()
//{
//	string filename="samples/lena.jpg";
//	//visit_elem(filename);
//	wscan(filename);
//	return 0;
//}