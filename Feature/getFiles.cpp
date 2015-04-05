#include <io.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

/*
*  功能：遍历文件和子文件夹内的文件
*  参数：
*  状态：
*/
void getFiles(string path, vector<string>& files)
{
	//文件句柄
	long  hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*.JPG").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之,如果不是,加入列表
			if ((fileinfo.attrib &_A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

//功能测试区
//int main()
//{
//	string path = "G:\\DataSET\\TextDetection\\MSRA-TD500\\test";
//	vector<string> filenames;
//	getFiles(path, filenames);
//	for (vector<string>::iterator it = filenames.begin(); it < filenames.end(); it++)
//	{
//		cout << *it << endl;
//		//mserArea(*it);
//		cin.get();
//	}
//
//	return 0;
//}