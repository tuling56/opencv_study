// 参考：http://blog.csdn.net/raodotcong/article/details/5982846

#include <stdio.h> 
#include <cv.h>

/** 定义的矩阵维数 */
const unsigned int M = 4;
const unsigned int N = 3;

/** 声明打印矩阵的函数 */  
void displayMatrix( const CvMat *mat ); 

int CalcVariance() 
{     
    /** 分配NxN的矩阵空间 */   
    CvMat* mat = cvCreateMat( M, N, CV_64FC1 );    
    
    /** 将矩阵中的元素初始为0-N的符合均匀分布的随机数 */    
    CvRandState randState;     
    cvRandInit( &randState, 0, N, cvGetTickCount(), CV_RAND_UNI ); 
    cvRand( &randState, mat );   

    /** 打印出原始的矩阵 */    
    printf( "Origin Matrix \n" );  
    displayMatrix( mat );     
    
    /** 分配协方差矩阵的空间 */     
    CvMat* covarianceOfMat = cvCreateMat( N, N, CV_64FC1 ); 
    cvZero( covarianceOfMat );  
    
    /** 计算协方差矩阵，用此方法计算得到的结果与Matlab中的成倍数关系，每个元素除以（行数-1）即可得到相同结果 */   
    cvCalcCovarMatrix( (const void **)&mat, 1, covarianceOfMat, NULL, CV_COVAR_NORMAL | CV_COVAR_ROWS /*  | CV_COVAR_SCALE 未使用自带归一化功能 */ );  
    
    /** 打印出归一化前的协方差矩阵 */   
    printf( "Covariance Matrix Before Equalization \n" );     
    displayMatrix( covarianceOfMat );   
    
    /** 进行归一化操作 */  
    if( M > 1 )  
    {      
        /** 每个元素除以（行数-1） */    
        cvConvertScale( covarianceOfMat, covarianceOfMat, 1.0 / ( M - 1 ) );   
    }  

    /** 打印出归一化后的协方差矩阵 */  
    printf( "Covariance Matrix After Equalization \n" );   
    displayMatrix( covarianceOfMat );     
    
    /** 释放为原始矩阵与协方差矩阵分配的空间 */   
    cvReleaseMat( &mat );    
    cvReleaseMat( &covarianceOfMat );   

    /** 等待用户输入一个字符后退出程序 */    
    getchar();       
    return 0;  
}   

/** 显示矩阵的中元素 */ 
void displayMatrix( const CvMat *mat ) 
{      
    size_t i, j;    

    /** 按照行与列的顺序打印出矩阵中的元素 */  
    for ( i = 0; i != mat->rows; i++ )     
    {        
        for ( j = 0; j != mat->cols; j++ )   
        {             
            printf( "%.4lf ",cvmGet( mat, i, j ) );     
        }         
        printf( "\n" );  
    }   
    printf( "\n" );
}