/* License:
   Oct. 3, 2008
   Right to use this code in any way you want without warrenty, support or any guarentee of it working.

   BOOK: It would be nice if you cited it:
   Learning OpenCV: Computer Vision with the OpenCV Library
     by Gary Bradski and Adrian Kaehler
     Published by O'Reilly Media, October 3, 2008
 
   AVAILABLE AT: 
     http://www.amazon.com/Learning-OpenCV-Computer-Vision-Library/dp/0596516134
     Or: http://oreilly.com/catalog/9780596516130/
     ISBN-10: 0596516134 or: ISBN-13: 978-0596516130    

   OTHER OPENCV SITES:
   * The source code is on sourceforge at:
     http://sourceforge.net/projects/opencvlibrary/
   * The OpenCV wiki page (As of Oct 1, 2008 this is down for changing over servers, but should come back):
     http://opencvlibrary.sourceforge.net/
   * An active user group is at:
     http://tech.groups.yahoo.com/group/OpenCV/
   * The minutes of weekly OpenCV development meetings are at:
     http://pr.willowgarage.com/wiki/OpenCV
*/

// alphablend <imageA> <image B> <x> <y> <width> <height> 
//            <alpha> <beta>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

int roi_merge()
{
    IplImage *src1, *src2;
	const char*src1name="PicVideo//happycat.png";
    const char*src2name="PicVideo//HandIndoorColor.jpg";
    if(((src1=cvLoadImage(src1name,1)) != 0)&&((src2=cvLoadImage(src2name,1)) != 0 ))
    {
		/* int x = atoi(argv[3]);
		int y = atoi(argv[4]);
		int width = atoi(argv[5]);
		int height = atoi(argv[6]);
		double alpha = (double)atof(argv[7]);
		double beta  = (double)atof(argv[8]);*/

		int x =120;
		int y = 120;
		int width = 300;
		int height = 300;
		double alpha = 0.6;
		double beta  = 0.4;


        cvSetImageROI(src1, cvRect(x,y,width,height));
        cvSetImageROI(src2, cvRect(0,0,width,height));
        cvAddWeighted(src1, alpha, src2, beta,0.0,src1);
        
		//cvResetImageROI(src1);
        cvNamedWindow( "Alpha_blend", 1 );
        cvShowImage( "Alpha_blend", src1 );
        
		cvWaitKey();
    }
    else 
      printf("Couldn't load one or both of %s, %s\n",src1name,src2name);
    return 0;
}
