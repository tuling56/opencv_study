#include<iostream>
#include<opencv2/core/core.hpp> //基本数据结构
#include<opencv2/highgui/highgui.hpp> //读写读取绘制

using namespace std;
using namespace cv;

int Data_Structure_main()
{
    /*深度：enum { CV_8U=0, CV_8S=1, CV_16U=2, CV_16S=3, CV_32S=4, CV_32F=5, CV_64F=6 };
    M.data  数据区域的指针
    M.dims  矩阵维度
    M.sizes  维度

    Range 范围，（start, end）
    */
    Mat mat_a(3,4,CV_64FC3,Scalar(255,0,0));  //定义一个3*4的矩阵，通道1
    Mat mat_b=mat_a(Range::all(),Range(1,3));
    cout<<"mat_a:"<<endl<<mat_a<<endl;
	cout<<mat_a.depth()<<","<<mat_a.channels()<<endl;
    cout<<"mat_b"<<endl<<mat_b<<endl;
	//imshow("MAT_A",mat_a);
	
    
    //typedef Point2i Point;
    Point2i point2_start(1,2);
	Point2i point1_start(22,30);
    Point2i point2_w_h(200,300);
    Point2i point2_w_hr(300,200);
    Point3f point3_start(10,20,30);
    Point2d point2_end(100,200); 
    /*
    rect = rect ± point //矩形偏移(shifting a rectangle by a certain offset)
    rect = rect ± size //改变大小(expanding or shrinking a rectangle by a certain amount)
    rect += point, rect -= point, rect += size, rect -= size //(augmenting operations)
    rect = rect1 & rect2 //矩形交集(rectangle intersection)
    rect = rect1 | rect2 //包含r1r2的最小矩形(minimum area rectangle containing rect2 and rect3 )
    rect &= rect1, rect |= rect1 //(and the corresponding augmenting operations)
    rect == rect1, rect != rect1 //(rectangle comparison)
    */
    Rect rect_a=Rect(point1_start,point2_w_h);
    Rect rect_b=Rect(point2_start,point2_w_hr); //旋转
    Rect rect_c=rect_a &rect_b;
	RotatedRect rRect=RotatedRect(point1_start,Size2f(50,100),30); //旋转矩形

    Mat img=imread("samples/lena.jpg");
    /*scalar是四维向量，至少应有两个参数Scalar_(_Tp v0, _Tp v1, _Tp v2=0, _Tp v3=0);
      rectangle在图像img_a上画一个矩形框
    */
    Scalar sa=Scalar(240,0,0);
    Scalar sb=Scalar(0,240,0);
    Scalar sc=Scalar(0,0,240);
    rectangle(img,rect_a.tl(),rect_a.br(),sa,1); 
    rectangle(img,rect_b.tl(),rect_b.br(),sb,1); 
    rectangle(img,rect_c.tl(),rect_c.br(),sc,3); 
	/*rectangle(img,rRect.tl(),)如何绘制旋转矩形*/ 
    //imshow("first image",img);
    //waitKey();
 
    /*Matx 小矩阵
    typedef Matx<float, 1, 2> Matx12f;
    typedef Matx<double, 1, 2> Matx12d;
    ...
    */
    // Matx12f matx12_f=Matx<float,1,2>(1,2);
	Matx32f matx12_f=Matx32f(1,2,4,5,6,7);
    cout<<matx12_f<<endl;
 
    /*Vec  短向量，基于Matx
    template<typename_Tp, int n> class Vec : public Matx<_Tp, n, 1> {...};
    typedef Vec<uchar, 2> Vec2b;
    typedef Vec<uchar, 3> Vec3b;
    typedef Vec<int, 2> Vec2i;
    typedef Vec<int, 3> Vec3i;
    typedef Vec<int, 4> Vec4i;
    */
    Vec2i v2i(1,20);
    cout<<v2i<<endl;

	//Mat
	Mat image(50,30,CV_8UC3,Scalar(255,0,0));
	imshow("Mat", image);
//	randu(image, Scalar(0), Scalar(256));
//	randu(image, Scalar(128), Scalar(10));
	Mat c = Mat::ones(3, 3, CV_32F) + 5;
	Mat d(20, 20, CV_32F);
	d = Scalar(25);

	waitKey();

    return 0;
}