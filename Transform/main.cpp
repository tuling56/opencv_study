//图像变换的主入口

#include <stdio.h>
#include "transform.h"


int main_()
{
	//-----------------------------------【仿射变换】-----------------------------
	//affine();
	
	//-----------------------------------【透视变换】-----------------------------
	//perspective();
	
	//四边形目标自动透视矫正
	//rectAutoRectify();  	
	homograpyPerspective();
		
	getchar();
	return 0;
}