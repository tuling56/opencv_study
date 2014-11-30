#include "kalman.h"
#include "ui_kalman.h"
#include <iostream>

using namespace std;

kalman::kalman(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kalman)
{
    //在构造函数中定义变量不行？
    img.create(500, 500, CV_8UC3);
    cout<<img.rows<<endl;
    //状态维数2，测量维数1，没有控制量
    KF.init(2, 1, 0);
    //状态值，（角度，角速度）
    state.create(2, 1, CV_32F);
    processNoise.create(2, 1, CV_32F);
    measurement = Mat::zeros(1, 1, CV_32F);
    timer   = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(kalman_process()));

    ui->setupUi(this);
    //这句必须放在ui->setupUi(this)后面，因为只有这样才能访问ui->textBrowser
    ui->textBrowser->setFixedSize(500, 500);
}

kalman::~kalman()
{
    delete ui;
}

void kalman::on_startButton_clicked()
{
    /*
      使用kalma步骤一
      下面语句到for前都是kalman的初始化过程，一般在使用kalman这个类时需要初始化的值有：
      转移矩阵，测量矩阵，过程噪声协方差，测量噪声协方差，后验错误协方差矩阵，
      前一状态校正后的值，当前观察值
    */

    //产生均值为0，标准差0.1的二维高斯列向量
    randn( state, Scalar::all(0), Scalar::all(0.1) );
    //transitionMatrix为类KalmanFilter中的一个变量，Mat型，是Kalman模型中的状态转移矩阵
    //转移矩阵为[1,1;0,1],2*2维的
    KF.transitionMatrix = *(Mat_<float>(2, 2) << 1, 1, 0, 1);

    //函数setIdentity是给参数矩阵对角线赋相同值，默认对角线值值为1
    setIdentity(KF.measurementMatrix);
    //系统过程噪声方差矩阵
    setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
    //测量过程噪声方差矩阵
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
    //后验错误估计协方差矩阵
    setIdentity(KF.errorCovPost, Scalar::all(1));
    //statePost为校正状态，其本质就是前一时刻的状态
    randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));
    //设置定时器时间,默认情况下其该定时器可无限定时，即其SingleShot为false
    //如果将其设置为true，则该定时器只能定时一次
    //因此这里是每个33ms就去执行一次kalman处理函数
    timer->start(33);
 //   timer->setSingleShot( true );

}


void kalman::kalman_process()
{
    Point2f center(img.cols*0.5f, img.rows*0.5f);
    float R = img.cols/3.f;
    //state中存放起始角，state为初始状态
    double stateAngle = state.at<float>(0);
    Point statePt = calcPoint(center, R, stateAngle);


    /*
      使用kalma步骤二
      调用kalman这个类的predict方法得到状态的预测值矩阵
    */
    //predicted state (x'(k)): x'(k)=A*x(k-1)+B*u(k)
    //其中x(k-1)为前面的校正状态，因此这里是用校正状态来预测的
    //而校正状态corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
    //又与本时刻的预测值和校正值有关系
    Mat prediction = KF.predict();
    //用kalman预测的是角度
    double predictAngle = prediction.at<float>(0);
    Point predictPt = calcPoint(center, R, predictAngle);

    randn( measurement, Scalar::all(0), Scalar::all(KF.measurementNoiseCov.at<float>(0)));

    // generate measurement
    //带噪声的测量
    measurement += KF.measurementMatrix*state;
   // measurement += KF.measurementMatrix*prediction;

    double measAngle = measurement.at<float>(0);
    Point measPt = calcPoint(center, R, measAngle);

    // plot points
    //这个define语句是画2条线段(线长很短)，其实就是画一个“X”叉符号
    #define drawCross( center, color, d )                                 \
        line( img, Point( center.x - d, center.y - d ),                \
                     Point( center.x + d, center.y + d ), color, 1, CV_AA, 0); \
        line( img, Point( center.x + d, center.y - d ),                \
                     Point( center.x - d, center.y + d ), color, 1, CV_AA, 0 )

    img = Scalar::all(0);
    //状态坐标白色
    drawCross( statePt, Scalar(255,255,255), 3 );
    //测量坐标蓝色
    drawCross( measPt, Scalar(0,0,255), 3 );
    //预测坐标绿色
    drawCross( predictPt, Scalar(0,255,0), 3 );
    //真实值和测量值之间用红色线连接起来
    line( img, statePt, measPt, Scalar(0,0,255), 3, CV_AA, 0 );
    //真实值和估计值之间用黄色线连接起来
    line( img, statePt, predictPt, Scalar(0,255,255), 3, CV_AA, 0 );


    /*
      使用kalma步骤三
      调用kalman这个类的correct方法得到加入观察值校正后的状态变量值矩阵
    */
    //虽然该函数有返回值Mat，但是调用该函数校正后，其校正值已经保存在KF的statePost
    //中了，corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
    KF.correct(measurement);

    randn( processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
    //不加噪声的话就是匀速圆周运动，加了点噪声类似匀速圆周运动，因为噪声的原因，运动方向可能会改变
    state = KF.transitionMatrix*state + processNoise;

    imwrite("../kalman/img.jpg", img);
    ui->textBrowser->clear();
    //ui->textBrowser->setFixedSize(img.cols, img.rows);
    ui->textBrowser->append("<img src=../kalman/img.jpg>");
}


void kalman::on_closeButton_clicked()
{
    close();
}
