//Tools for Labeling Video
//Coded by L. Wei
//Date: 9/4/2013
/*
 *功能：视频标注和标注结果保存成配置文件
 *来自：http://blog.csdn.net/xiaowei_cqu/article/details/8778976
 *Satus:此功能包含配置文件的写和读
 *Info:[11/12/2014 jmy]
 */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//Global variables
bool is_drawing=false;
vector<Rect> biaozhu_boxs; //所有的标注矩形框
Rect drawing_box;
Mat img_original,img_drawing;

CvFileStorage *fs; //配置文件
typedef struct UnitConf
{
	char meaning[20];
	Rect loc;
} s_unitInfo;
s_unitInfo meanloc; //每个区域的意义和位置
vector<s_unitInfo> config;

//说明
static void help()
{
	cout << "This program designed for labeling video \n"
		<<"Coded by L. Wei on 9/4/2013\n"<<endl;
	
	cout<<"Use the mouse to draw rectangle on the image for labeling.\n"<<endl;

	cout << "Hot keys: \n"
		"\tESC - quit the program\n"
		"\tSpace Key - pause/start\n"
		"\tz - undo the last label\n"
		"\tc - clear all the labels\n"
		<<endl;
}

/*
 *	功能：写XML文件
 *  参数：
 *  状态：
 */
static int xml_write(char* meaning, const Rect loc)
{
	
   // 开始写结构，类型是图map，也就是有名字的无序节点集合  
	/*
	const char *attr[4]={"imagename=photoname","width=1","height=2","AreaNum=9"};
	CvAttrList imageinfo=cvAttrList(attr,0);
	*/
	cvStartWriteStruct(fs, meaning, CV_NODE_MAP, meaning, cvAttrList(0, 0));
	//cvWriteComment(fs, "text area info", 1);
	//cvWriteString(fs, "Object", meaning);
	cvWriteInt(fs, "x", loc.tl().x);
	cvWriteInt(fs, "y", loc.tl().y);
	cvWriteInt(fs, "width", loc.width);
	cvWriteInt(fs, "height", loc.height);

	//序列结构部分
	//cvWriteInt(fs, "loc", 4);
	int area_info[4] = { loc.tl().x, loc.tl().y, loc.width, loc.height };
	cvStartWriteStruct(fs, "loc", CV_NODE_SEQ);  //开始写结构，类型是序列sequence，无名字的有序节点集合  
	cvWriteRawData(fs, area_info, 4, "i");
	cvEndWriteStruct(fs);

	cvEndWriteStruct(fs);

	return 0;
}

/*
 *	功能：读xml文件
 *  参数：
 *  状态：
 */
static Rect xml_read(CvFileStorage *fs,char*meaning)
{
	CvFileNode * node;
	
	//获得第一层数据节点
	node = cvGetFileNodeByName(fs, 0, meaning);
	//char * str= const_cast<char*>(cvReadStringByName(fs, node, "loc"));
	printf("\nName=%s", meaning);
		
	//序列结构的读取，例如 <sales_record> 30000 4200 50090 6100</sales_record>
	int* d = (int*)cvAlloc(sizeof(int)*4);
	if (d)
	{
		node = cvGetFileNodeByName(fs, node, "loc");
		if (node)
		{
			cvReadRawData(fs, node, d, "i");
			printf("\nloc=");
			for (int i = 0; i < 4; i++)
				printf("%d ", d[i]);
		}
		//cvFree(&d);
	}
	Rect locinfo=Rect(d[0], d[1], d[2], d[3]);
	cvFree(&d);
	return locinfo;
}

/*
 *	功能：回调函数，显示矩形框
 *  参数：
 *  状态：
 */
static void onMouse( int event, int x, int y, int, void* )
{
	switch(event)
	{
		case CV_EVENT_LBUTTONDOWN: 
			//the left up point of the rect
			is_drawing=true;
			drawing_box.x=x;
			drawing_box.y=y;
			break;
		case CV_EVENT_MOUSEMOVE:
			//adjust the rect (use color blue for moving)
			if(is_drawing)
			{
				drawing_box.width=x-drawing_box.x;
				drawing_box.height=y-drawing_box.y;
				img_original.copyTo(img_drawing);
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
				}
				rectangle(img_drawing,drawing_box,Scalar(255,0,0));
			}
			break;
		case CV_EVENT_LBUTTONUP:
			//finish drawing the rect (use color green for finish)
			if(is_drawing)
			{
				drawing_box.width=x-drawing_box.x;
				drawing_box.height=y-drawing_box.y;
				img_original.copyTo(img_drawing);
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
				}
				rectangle(img_drawing,drawing_box,Scalar(0,255,0));
				cout << "Please input the meaning" << endl;
				//gets(meaning);
				cin >>meanloc.meaning;
				meanloc.loc = Rect(x, y, drawing_box.width, drawing_box.height);
				config.push_back(meanloc);
				biaozhu_boxs.push_back(drawing_box);
			}
			is_drawing=false;
			break;
	}
	imshow("Video",img_drawing);
	return;
}

/*
 *	功能：视频标注函数主入口
 *  说明：每标注一个区域，输入对应的代表位置信息，按Enter键确认
 *  状态：
 */
int videolabel(char*videoname,char*result)
{
	help();
	ofstream outfile(result);

	//原始视频显示
	namedWindow("Video");
	VideoCapture capture(videoname);
	capture>>img_original; 
	imshow("Video",img_original);
	
	setMouseCallback( "Video", onMouse, 0 ); //设置回调函数

	//对被标注的第一帧的处理,绘制所有矩形（这一步存在问题）
	img_original.copyTo(img_drawing);
	for(vector<Rect>::iterator it=biaozhu_boxs.begin();it!=biaozhu_boxs.end();++it)
	{
		 cout<<"开始的时候哪有矩形要绘制"<<endl;
	     rectangle(img_drawing,(*it),Scalar(0,255,0));
		 outfile<<"[1]"<<(*it).x<<" "<<(*it).y<<" "<<(*it).width<<" "<<(*it).height<<endl;
	}
	
	int frame_counter=1;
	while(1)
	{
		capture >> img_original;  //read the next frame
		int c=waitKey(10);
		if( (c & 255) == 27 ){   //ESC键退出
			cout << "Exiting ...\n";
			break;
		}
		switch((char)c)
		{
		     case ' ':
				++frame_counter;
				capture>>img_original;  //read the next frame
				if(img_original.empty()){
					cout<<"\nVideo Finished!"<<endl;
					return 0;
				}
				waitKey();
				img_original.copyTo(img_drawing);
				//save and draw all of the labeling rects
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
					outfile<<"["<<frame_counter<<"]"<<(*it).x<<" "<<(*it).y<<" "<<(*it).width<<" "<<(*it).height<<endl;
				}
				break;
			case 'z':
				//undo the latest labeling
				if(!biaozhu_boxs.empty())
				{
					vector<Rect>::iterator it_end=biaozhu_boxs.end();
					--it_end;
					biaozhu_boxs.erase(it_end);
				}
				img_original.copyTo(img_drawing);//把上次读帧弄过来，在新帧上不再显示
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
				}
				break;

			case 'c':
				//clear all the rects on the image
				biaozhu_boxs.clear();
				img_original.copyTo(img_drawing);
			//default:
				//waitKey();
		}

		for (vector<Rect>::iterator it = biaozhu_boxs.begin(); it != biaozhu_boxs.end(); ++it)
		{
			rectangle(img_original, (*it), Scalar(0, 255, 0));
		}
	   img_original.copyTo(img_drawing);
	   imshow("Video",img_drawing);
	}

	return 0;
}



//功能测试区
int main(int argc,char**argv)
{
	char*videoname = "samples/video/test_o2.mp4";
	char *result = "config.txt";
	char*configname = "config.xml";
	
	//(1)视频标注
	videolabel(videoname, result);
	
	//(2)写配置文件  
	fs = cvOpenFileStorage(configname, 0, CV_STORAGE_WRITE, "UTF-8");
	cvWriteComment(fs, "config info xml file", 1);
	for (vector<s_unitInfo>::iterator it = config.begin(); it != config.end();++it)
	{
		xml_write(it->meaning, it->loc);
	}
	cvReleaseFileStorage(&fs);
    
	//（3）读配置文件
	fs = cvOpenFileStorage(configname, 0, CV_STORAGE_READ, "UTF-8");
	//xml_read(fs, "ww");
	char* means[20] = { "ww", "erz", "vv" }; //配置文件的意义名事先定义
	for (int i = 0; i < 20;++i)
    {
		if (means[i])
		{
			Rect loc=xml_read(fs, means[i]);
			strcpy(meanloc.meaning, means[i]);
			meanloc.loc = loc;
			config.push_back(meanloc);//最后所有的信息都被保存在config这个vector中
		}
		else
			break;
    }
	cvReleaseFileStorage(&fs);

	//(4)测试对区域的操纵
	//根据指定区域的识别结果，删减对应的位置信息，


	getchar();
	return 0;
}