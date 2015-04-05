//-----------------------------------【程序说明】----------------------------------------------
//		程序名称:：《【OpenCV入门教程之十五】水漫金山：OpenCV漫水填充算法（Floodfill）》 博文配套源码 
//		开发所用IDE版本：Visual Studio 2010
//   		开发所用OpenCV版本：	2.4.9
//		2014年6月3日 Created by 浅墨
//		浅墨的微博：@浅墨_毛星云 http://weibo.com/1723155442/profile?topnav=1&wvr=5&user=1
//		来源：http://blog.csdn.net/poem_qianmo/article/details/28261997
//      说明：该程序包含键盘，鼠标，滑动条等事件处理函数，另外还牵涉到掩膜处理，非常全面
//----------------------------------------------------------------------------------------------

//-----------------------------------【头文件包含部分】---------------------------------------  
//      描述：包含程序所依赖的头文件  
//----------------------------------------------------------------------------------------------
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

//-----------------------------------【命名空间声明部分】---------------------------------------  
//      描述：包含程序所使用的命名空间  
//-----------------------------------------------------------------------------------------------   
using namespace cv;
using namespace std;


//-----------------------------------【全局变量声明部分】--------------------------------------  
//      描述：全局变量声明  
//-----------------------------------------------------------------------------------------------  
//定义原始图、目标图、灰度图、掩模图，分割后的图像
Mat g_srcImage, g_dstImage, g_grayImage, g_maskImage, g_SegmentImage;   
int g_nFillMode = 1;   //漫水填充的模式，初始设置为渐变固定的填充模式
int g_nLowDifference = 20, g_nUpDifference = 20;//负差最大值、正差最大值
int g_nConnectivity = 4;//表示floodFill函数标识符低八位的连通值
int g_bIsColor = true;//是否为彩色图的标识符布尔值，这样会存在bug的，若原始图像是灰度图，在转化为灰度图的时候就会出问题
bool g_bUseMask = false;//是否显示掩膜窗口的布尔值
int g_nNewMaskVal = 255;//新的重新绘制的像素值，绘制的是掩码图像的值


//-----------------------------------【ShowHelpText( )函数】----------------------------------  
//      描述：输出一些帮助信息  
//----------------------------------------------------------------------------------------------  
static void ShowHelpText()  
{  
	//输出一些帮助信息  
	printf("\n\n\n\t欢迎来到漫水填充示例程序~\n\n");  
	printf( "\n\n\t按键操作说明: \n\n"  
		"\t\t鼠标点击图中区域- 进行漫水填充操作\n"  
		"\t\t键盘按键【ESC】- 退出程序\n"  
		"\t\t键盘按键【1】-  切换彩色图/灰度图模式\n"  
		"\t\t键盘按键【2】- 显示/隐藏掩膜窗口\n"  
		"\t\t键盘按键【3】- 恢复原始图像\n"  
		"\t\t键盘按键【4】- 使用空范围的漫水填充\n"  
		"\t\t键盘按键【5】- 使用渐变、固定范围的漫水填充\n"  
		"\t\t键盘按键【6】- 使用渐变、浮动范围的漫水填充\n"  
		"\t\t键盘按键【7】- 操作标志符的低八位使用4位的连接模式\n"  
		"\t\t键盘按键【8】- 操作标志符的低八位使用8位的连接模式\n"  
		"\n\n\t\t\t\t\t\t\t\t by浅墨\n\n\n"  
		);  
}  


//-----------------------------------【onMouse( )函数】--------------------------------------  
//      描述：鼠标消息onMouse回调函数
//---------------------------------------------------------------------------------------------
static void onMouse( int event, int x, int y, int, void* )
{
	// 若鼠标左键没有按下，便返回
	if( event != CV_EVENT_LBUTTONDOWN )
		return;

	//-------------------【<1>调用floodFill函数之前的参数准备部分】---------------
	Point seed = Point(x,y);
	
	//控制填充模式
	int LowDifference = (g_nFillMode == 0) ? 0 : g_nLowDifference;//空范围的漫水填充，此值设为0，否则设为全局的g_nLowDifference
	int UpDifference = (g_nFillMode == 0) ? 0 : g_nUpDifference;//空范围的漫水填充，此值设为0，否则设为全局的g_nUpDifference
	
	
	//标识符的0~7位为g_nConnectivity，8~15位为g_nNewMaskVal左移8位的值，16~23位为CV_FLOODFILL_FIXED_RANGE或者0。
	int flags = g_nConnectivity + (g_nNewMaskVal << 8) + //g_nNewMaskVal用来指定填充掩膜图像的至
		(g_nFillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);//g_nFillMode用来控制是否考虑与相邻像素的差，渐变固定范围，考虑当前像素与种子像素的差；否则，就考虑当前像素与相邻像素的差，这个范围是浮动的
	

	//随机生成bgr值
	int b = (unsigned)theRNG() & 255;//随机返回一个0~255之间的值
	int g = (unsigned)theRNG() & 255;//随机返回一个0~255之间的值
	int r = (unsigned)theRNG() & 255;//随机返回一个0~255之间的值
	Rect ccomp;//定义重绘区域的最小边界矩形区域

	//在重绘区域像素的新值，若是彩色图模式，取Scalar(b, g, r)；若是灰度图模式，取Scalar(r*0.299 + g*0.587 + b*0.114)
	Scalar newVal = g_bIsColor ? Scalar(b, g, r) : Scalar(r*0.299 + g*0.587 + b*0.114);
	Mat dst = g_bIsColor ? g_dstImage : g_grayImage;//目标图的赋值
	
	int area;

	//--------------------【<2>正式调用floodFill函数】-----------------------------
	if( g_bUseMask )
	{
		threshold(g_maskImage, g_maskImage, 1, 128, CV_THRESH_BINARY); //二值化掩膜
		//floodFill返回的是被重绘的像素个数，同时也修改了掩码图像
		area = floodFill(dst, g_maskImage, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference),
			Scalar(UpDifference, UpDifference, UpDifference), flags);
		imshow( "mask", g_maskImage );
	}
	else
	{
		//不使用掩膜的时候，按灰度图像处理，根据亮度差调整
		area = floodFill(dst, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference),
			Scalar(UpDifference, UpDifference, UpDifference), flags);
	}

	imshow("效果图", dst); //效果图就是经过漫水算法之后的图像
	cout << area << " 个像素被重绘\n"; 
}


//-----------------------------------【main( )函数】--------------------------------------------  
//      描述：控制台应用程序的入口函数，我们的程序从这里开始  
//-----------------------------------------------------------------------------------------------  
int floodFill_main( int argc, char** argv )
{
	//改变console字体颜色  
	system("color 2F");    
	//载入原图
	g_srcImage = imread("samples/text_dete/text_img0265.png", 1);
	if( !g_srcImage.data ) {
		printf("Oh，no，读取图片image0错误~！ \n"); 
		return false; 
	}  

	//显示帮助文字
	ShowHelpText();

	g_srcImage.copyTo(g_dstImage);//拷贝源图到目标图
	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);//转换三通道的彩色图到灰度图
	//问题就出在这个掩码的
	g_maskImage.create(g_srcImage.rows+2, g_srcImage.cols+2, CV_8UC1);//利用原始图像的尺寸来初始化掩膜mask，掩膜图像的修改是在漫水算法中

	namedWindow( "效果图",CV_WINDOW_AUTOSIZE );
	namedWindow("SegmentImage", CV_WINDOW_AUTOSIZE);
	g_maskImage = Scalar::all(255);

    //创建Trackbar
	createTrackbar( "负差最大值", "效果图", &g_nLowDifference, 255, 0 );
	createTrackbar( "正差最大值" ,"效果图", &g_nUpDifference, 255, 0 );

	//鼠标回调函数
	setMouseCallback( "效果图", onMouse, 0 );

	//循环轮询按键
	while(1)
	{
		//先显示效果图
		imshow("效果图", g_bIsColor ? g_dstImage : g_grayImage);
		imshow("test", g_maskImage);
		//g_srcImage.copyTo(g_SegmentImage, g_maskImage); //在这部调用的时候，g_maskImage为空,问题就出在这，掩码图像必须为灰度图像
	    //imshow("SegmentImage", g_SegmentImage);
 #pragma region key
	//获取键盘按键
		int c = waitKey(0);
		//判断ESC是否按下，若按下便退出
		if( (c & 255) == 27 )
		{
			cout << "程序退出...........\n";
			break;
		}
#pragma endregion key

#pragma region keyboard
//根据按键的不同，进行各种操作
		switch( (char)c )
		{
			//如果键盘“1”被按下，效果图在在灰度图，彩色图之间互换
		case '1':
			if( g_bIsColor )//若原来为彩色，转为灰度图，并且将掩膜mask所有元素设置为0
			{
				cout << "键盘“1”被按下，切换彩色/灰度模式，当前操作为将【彩色模式】切换为【灰度模式】\n";
				cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
				g_maskImage = Scalar::all(0);	//将mask所有元素设置为0
				g_bIsColor = false;	//将标识符置为false，表示当前图像不为彩色，而是灰度
			}
			else//若原来为灰度图，便将原来的彩图image0再次拷贝给image，并且将掩膜mask所有元素设置为0
			{
				cout << "键盘“1”被按下，切换彩色/灰度模式，当前操作为将【彩色模式】切换为【灰度模式】\n";
				g_srcImage.copyTo(g_dstImage);
				g_maskImage = Scalar::all(0);
				g_bIsColor = true;//将标识符置为true，表示当前图像模式为彩色
			}
			break;
			//如果键盘按键“2”被按下，显示/隐藏掩膜窗口,初始的时候，掩码窗口都是黑色的
		case '2':
			if( g_bUseMask )
			{
				destroyWindow( "mask" );
				g_bUseMask = false;
			}
			else
			{
				namedWindow( "mask", 0 );
				g_maskImage = Scalar::all(0); //掩膜窗口置黑
				imshow("mask", g_maskImage); //掩膜窗口切换
				g_bUseMask = true;
			}
			break;
			//如果键盘按键“3”被按下，恢复原始图像
		case '3':
			cout << "按键“3”被按下，恢复原始图像\n";
			g_srcImage.copyTo(g_dstImage);
			cvtColor(g_dstImage, g_grayImage, COLOR_BGR2GRAY);
			g_maskImage = Scalar::all(0);
			if (g_bUseMask) //若此时是显示掩膜窗口状态，则更像掩膜窗口
			{
				imshow("mask", g_maskImage); //掩膜窗口切换
			}
			break;
			//如果键盘按键“4”被按下，使用空范围的漫水填充
		case '4':
			cout << "按键“4”被按下，使用空范围的漫水填充\n";
			g_nFillMode = 0; //0差容忍，就是只选择和种子点相同的区域，而不是相似的区域
			break;
			//如果键盘按键“5”被按下，使用渐变、固定范围的漫水填充
		case '5':
			cout << "按键“5”被按下，使用渐变、固定范围的漫水填充\n";
			g_nFillMode = 1; //仅考虑种子点
			break;
			//如果键盘按键“6”被按下，使用渐变、浮动范围的漫水填充
		case '6':
			cout << "按键“6”被按下，使用渐变、浮动范围的漫水填充\n";
			g_nFillMode = 2; //不仅考虑种子点，也考虑相邻像素
			break;
			//如果键盘按键“7”被按下，操作标志符的低八位使用4位的连接模式
		case '7':
			cout << "按键“7”被按下，操作标志符的低八位使用4位的连接模式\n";
			g_nConnectivity = 4;
			break;
			//如果键盘按键“8”被按下，操作标志符的低八位使用8位的连接模式
		case '8':
			cout << "按键“8”被按下，操作标志符的低八位使用8位的连接模式\n";
			g_nConnectivity = 8;
			break;
		}
#pragma endregion keyboard process
	}

	return 0;
}