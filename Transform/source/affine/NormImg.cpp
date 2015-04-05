#include"../test.h";

void normalizeImage (IplImage * input, IplImage * output) 
{
	assert ( input->depth == IPL_DEPTH_32F );
	assert ( input->nChannels == 1 );
	assert ( output->depth == IPL_DEPTH_32F );
	assert ( output->nChannels == 1 );

	float maxVal = 0;
	float minVal = 1e100;
	for( int row = 0; row < input->height; row++ )
	{
		const float* ptr = (const float*)(input->imageData + row * input->widthStep);
		for ( int col = 0; col < input->width; col++ )
		{
			if (*ptr < 0) { }
			else {
				maxVal = std::max(*ptr, maxVal);
				minVal = std::min(*ptr, minVal);
			}
			ptr++;
		}
	}

	float difference = maxVal - minVal;
	for( int row = 0; row < input->height; row++ )
	{
		const float* ptrin = (const float*)(input->imageData + row * input->widthStep);
		float* ptrout = (float*)(output->imageData + row * output->widthStep);
		for ( int col = 0; col < input->width; col++ )
		{
			if (*ptrin < 0) {
				*ptrout = 1;
			} else {
				*ptrout = ((*ptrin) - minVal)/difference;
			}
			ptrout++;
			ptrin++;
		}
	}
}

int normalizeImageShow( char *filename )
{
   IplImage* input=NULL;
   if((input= cvLoadImage( filename, 0)) != 0 )
  {   
	  //IplImage* input=cvLoadImage(filename,0);
	  IplImage* floatinput=cvCreateImage (cvGetSize ( input ),IPL_DEPTH_32F, 1 );
	  cvConvertScale ( input, floatinput, 1 / 255., 0 );
	  IplImage*output=cvCreateImage (cvGetSize ( input ),IPL_DEPTH_32F, 1);
	  normalizeImage(floatinput,output);
	  cvShowImage("input",input);
	  cvShowImage("output",output);
   }
      // cvWaitKey();
  return 0;
}