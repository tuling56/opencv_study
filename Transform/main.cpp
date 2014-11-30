//几何变换的主入口

#include <stdio.h>
#include "transform.h"


int main()
{
	/*仿射变换*/
	//affine();
	
	/*透视*/
	//perspective();
	 
	/*四边形目标自动透视矫正*/
	rectAutoRectify();  
		
	getchar();
	return 0;
}