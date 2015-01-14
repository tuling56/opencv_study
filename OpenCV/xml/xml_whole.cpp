/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	xml_whole.cpp
* Brief: OpenCV xml读写
* Source:http://blog.csdn.net/augusdi/article/details/8865515
* Status: 未完成调通测试
* Date:	[1/14/2015 jmy]
************************************************************************/


#include <cxcore.h>
#include <highgui.h>

//#include <opencv2/core/core.hpp>


int sub_test_opencv_xml_write(void)
{
	// 创建文件存储对象
	CvFileStorage *fs=cvOpenFileStorage("test.xml",0,CV_STORAGE_WRITE);
	// 写注释
	cvWriteComment(fs,"测试写XML文件",1);
	
	// 开始写结构，类型是图map，也就是有名字的无序节点集合
	cvStartWriteStruct(fs,"Employee",CV_NODE_MAP);
	// 注释
	cvWriteComment(fs,"MAP类型，姓名，年龄，薪水",1);
	// 姓名
	cvWriteString(fs,"name","刘越",0);
	// 年龄
	cvWriteInt(fs,"age",18);
	// 薪水
	cvWriteReal(fs,"salary",2780.3);
	// 销售次数
	cvWriteInt(fs,"sales_count",4);
	{
		// 销售具体数据
		int sales_record[]={30000,4200,50090};
		// 注释
		cvWriteComment(fs,"SEQ类型，销售记录",1);
		// 开始写结构，类型是序列sequence，无名字的有序节点集合
		cvStartWriteStruct(fs,"sales_record",CV_NODE_SEQ);
		// 前3条销售记录
		cvWriteRawData(fs,sales_record,3,"i");
		// 第4条销售记录，注意无名字
		cvWriteInt(fs,0,6100);
		// 结束
		cvEndWriteStruct(fs);
	}
	{
		// 注释
		cvWriteComment(fs,"MAP类型，亲友",1);
		// 开始
		cvStartWriteStruct(fs,"Parent",CV_NODE_MAP);
		// 父亲
		cvWriteString(fs,"father","杨舜");
		// 母亲
		cvWriteString(fs,"mother","王娟");
		// 结束
		cvEndWriteStruct(fs);
	}
	// 结束
	cvEndWriteStruct(fs);
	// 释放文件存储对象
	cvReleaseFileStorage(&fs);
}

int sub_test_opencv_xml_read(void)
{
	// 文件节点
	CvFileNode * node, *node2;
	char * str;
	int count;
	int * d;
	
	//cve_dm.debug_break();
	// 打开XML文件
	CvFileStorage *fs = cvOpenFileStorage("test.xml",0,CV_STORAGE_READ);
	// 获得第一层数据节点
	node = cvGetFileNodeByName(fs,0,"Employee");
	str = cvReadStringByName(fs,node,"name");
	printf("\n姓名=%s",str);
	printf("\n年龄=%d",cvReadIntByName(fs,node,"age"));
	printf("\n薪水=%g",cvReadRealByName(fs,node,"salary"));
	count = cvReadIntByName(fs,node,"sales_count");
	printf("\n销售%d条",count);
	d = cvAlloc(sizeof(int)*count);
	if(d)
	{
		int i;
		node2 = cvGetFileNodeByName(fs,node,"sales_record");
		if(node2)
		{
			cvReadRawData(fs,node2,d,"i");
			printf("\n销售记录=");
			for(i=0;i<count;i++)
				printf("%d, ",d[i]);
		}
		cvFree(&d);
	}
	// 获得第二层节点
	node = cvGetFileNodeByName(fs,node,"Parent");
	printf("\n根节点=%s",cvGetFileNodeName(node));
	str = cvReadStringByName(fs,node,"father");
	printf("\n父亲=%s",str);
	str = cvReadStringByName(fs,node,"mother");
	printf("\n母亲=%s",str);
}

//功能测试区
int main()
{
	sub_test_opencv_xml_write();
	return 0;
}