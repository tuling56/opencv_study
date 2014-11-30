#include <highgui.h>  
#include <cv.h>  
#include <iostream>  
#include <iomanip>  
#include "opencv.h"

using namespace std; 



/**********写XML***********/  
int xml_read_write()  
{  
	int a=1;  
	float b=2.;  
	double c[]={4.5,6.7,8.9};  
	
	CvFileStorage *fs=cvOpenFileStorage("test.xml",0,CV_STORAGE_WRITE);  
	cvWriteComment(fs,"text location",1);  
	
	//开始写数据 
	cvStartWriteStruct(fs,"DATA",CV_NODE_MAP,0,cvAttrList(0,0));   

	//写入整型数据a  
	cvWriteInt(fs,"a",a);  
	//写入浮点型数据b  
	cvWriteReal(fs,"b",b); 
	
	//写入数据c数组  
	cvStartWriteStruct(fs,"c",CV_NODE_SEQ,0,cvAttrList(0,0));  
	cvWriteRawData(fs,c,3,"d");  
	cvEndWriteStruct(fs);

	cvEndWriteStruct(fs); 
	cvReleaseFileStorage(&fs);  
	return 0;
	
} 

/**********读取XML***********/  
int xml_mat_read()
{
	int i,j;  
	CvMat *readbyname,*readbynode;  
	CvFileNode *mat_node;//文件结点  
	CvFileStorage *fs_read=NULL;//文件存储  
	fs_read=cvOpenFileStorage("mat.xml",0,CV_STORAGE_READ);  
	readbyname = (CvMat *)cvReadByName(fs_read,NULL,"mat",NULL);  
	mat_node = cvGetFileNodeByName(fs_read,NULL,"mat");  
	readbynode = (CvMat *)cvRead(fs_read,mat_node);  
	for (i=0;i<3;i++)  
	{  
		for (j=0;j<3;j++)  
		{  
			cout<<CV_MAT_ELEM(*readbynode,int,i,j);  
		}  
		cout<<endl;  
	}  
	cvReleaseFileStorage(&fs_read);  
	cvReleaseMat(&readbynode);  
	cvReleaseMat(&readbyname);  

	return 0;  

}

/**********写XML***********/  
int opencv_xml_write(char* photoname,const Localtion areainfo)  
{  
	// 创建文件存储对象  
	CvFileStorage *fs=cvOpenFileStorage("location.xml",0,CV_STORAGE_WRITE,"UTF-8");   
	cvWriteComment(fs,"test xml file",1);  

	// 开始写结构，类型是图map，也就是有名字的无序节点集合  
	/*
	const char *attr[4]={"imagename=photoname","width=1","height=2","AreaNum=9"};
	CvAttrList imageinfo=cvAttrList(attr,0);
	*/
	cvStartWriteStruct(fs,"Tagset",CV_NODE_MAP,"id_size",cvAttrList(0,0));  
	
	cvWriteComment(fs,"text area info",1);  
	cvWriteString(fs,"IMG",photoname);
	cvWriteInt(fs,"Height",18); 
	cvWriteInt(fs,"Width",18);  
 
	cvWriteInt(fs,"areas_num",4);  
	int area_info[4]={areainfo.lx,areainfo.ly,areainfo.rx,areainfo.ry};  
	// 开始写结构，类型是序列sequence，无名字的有序节点集合  
    cvStartWriteStruct(fs,"area_coor",CV_NODE_SEQ);  
	cvWriteRawData(fs,area_info,4,"i");   
	cvEndWriteStruct(fs);  
	 
 
	cvEndWriteStruct(fs);   
	cvReleaseFileStorage(&fs);  
	return 0;
}   
