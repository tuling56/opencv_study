//-----------------------------------【直方图】-----------------------------
//描述：直方图的计算，归一化和绘制，距离的计算
//-----------------------------------------------------------------------
int calcHistDistance();//来自cnblog
int oneDim(char *filename);
int mutiDim(char *filename);
void calcDrawHist(char* path);//计算和绘制直方图
int equalizeHist(char*path);//直方图均衡化
int colorHistgram( int argc, char** argv );