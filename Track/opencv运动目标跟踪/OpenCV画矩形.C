CvPoint2D32f point4[4];
  cvBoxPoints( track_box, point4);//得到track_box四个角的点的坐标
  CvPoint pt[4];
  for(int i=0;i<4;i++)
  pt[i]=cvPoint((int)point4[i].x,(int)point4[i].y);//把CvPoint2D32f类型的点转为CvPoint类型
  CvPoint* ppt=pt;

int count=4;
  cvPolyLine( imageTrack, &ppt,&count,1,1,CV_RGB(255,0,0),4);//画矩形
