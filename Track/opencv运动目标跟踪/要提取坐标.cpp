int i,j;
uchar* imgArray;
uchar* array;


imgArray=(uchar*)image->imageData;//图像数据的指针
for (i=0;i<image->height;i++)
{
array=imgArray+image->widthStep*i;//当前行的指针
for (j=0;j<image->width;j++)
{
if(array[j*3]>thr)//当前像素值大于阈值
prinrf("i=%d j=%d",i,j);//输出坐标
}
}
