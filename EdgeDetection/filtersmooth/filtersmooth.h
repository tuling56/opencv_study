//Function：滤波和平滑部分头文件
//Source:
//Status:
//Improve:
//Info:[11/16/2014 jmy]

int backwards();//逆向滤波
int opencv2_filter(char*path);//opencv自带的方框、中值和高斯和双边滤波
int linear_filter(char*path);//来自浅墨，包含方框，均值和高斯
int filter_whole( char*path );//来自浅墨，包含线性（方框，均值和高斯）和非线性（中值和双边）