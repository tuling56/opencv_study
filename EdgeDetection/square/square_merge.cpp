/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	square_merge.cpp
* Brief: 矩形框融合算法
* Source:http://www.tuicool.com/articles/7zqUJr6
* Status: 存在大量问题，未完成
* Date:	[3/22/2015 jmy]
************************************************************************/
#include<opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

/*-----------------------MeanShift做局部的（源程序是对全局）------------------------*/
//meanshift 融合
class MeanshiftGrouping
{
public:
	//	 MeanshiftGrouping msGrouping(smothing, hits,rectList, hitWeights, 1e-5, 100);//得到
	///////////////////////////////////////////////////////////////////////
	//	 msGrouping.getModes(resultHits, resultWeights, 1);
	//////////////////////////////////////////////////////////////////////
	MeanshiftGrouping(const Point3d& densKer, const vector<Point3d>& posV, const vector<Rect>&list,
		const vector<double>& wV, double eps, int maxIter = 20)
	{
		densityKernel = densKer;
		weightsV = wV;
		positionsV = posV;
		positionsCount = (int)posV.size();
		meanshiftV.resize(positionsCount);
		distanceV.resize(positionsCount);
		iterMax = maxIter;
		modeEps = eps;

		for (unsigned i = 0; i < positionsV.size(); i++)
		{
			meanshiftV[i] = getNewValue(positionsV[i], list[i], list);//positionV 只有中点坐标没有长宽。
			distanceV[i] = moveToMode(meanshiftV[i], list[i], list);//做最大为iterMax次循环//均值漂移后的值
			meanshiftV[i] -= positionsV[i];//这一步后面没用到
		}
	}
	void getModes(vector<Point3d>& modesV, vector<double>& resWeightsV, const double eps)
	{
		for (size_t i = 0; i < distanceV.size(); i++)
		{
			bool is_found = false;
			for (size_t j = 0; j < modesV.size(); j++)
			{
				if (getDistance(distanceV[i], modesV[j]) < eps)//欧式距离小于阈值
				{
					is_found = true;
					break;
				}
			}
			if (!is_found)
			{
				modesV.push_back(distanceV[i]);//添加距离较大的点，也就是说两个点距离较大，不是同一个矩形框
			}
		}

		resWeightsV.resize(modesV.size());

		for (size_t i = 0; i < modesV.size(); i++)
		{
			resWeightsV[i] = getResultWeight(modesV[i]);//得到点的权值
		}
	}

protected:
	vector<Point3d> positionsV;
	vector<double> weightsV;

	Point3d densityKernel;
	int positionsCount;

	vector<Point3d> meanshiftV;
	vector<Point3d> distanceV;
	int iterMax;
	double modeEps;

	Point3d getNewValue(const Point3d& inPt, const Rect inR, const vector<Rect>list) const
	{//inPt 输入三维坐标 、inR 为输入的矩形  list为所有矩形
		Point3d resPoint(.0);
		Point3d ratPoint(.0);
		int value = 20;//只考虑矩形框四个角差值小于20的点，这个可以自己设定
		for (size_t i = 0; i < positionsV.size(); i++)
		{
			Point3d aPt = positionsV[i];
			// double rate = exp(aPt.z);
			if (inR.x - list[i].x > value || inR.y - list[i].y > value || inR.width - list[i].width > value || inR.height - list[i].height > value)
				continue;//局部判断，如果不是同一个矩形附近，将排除附近矩形的影响

			Point3d bPt = inPt;
			Point3d sPt = densityKernel;//核


			////////////////////////////////////////
			sPt.x *= exp(aPt.z);//Z为尺度
			sPt.y *= exp(aPt.z);

			aPt.x /= sPt.x;
			aPt.y /= sPt.y;
			aPt.z /= sPt.z;

			bPt.x /= sPt.x;
			bPt.y /= sPt.y;
			bPt.z /= sPt.z;
			///映射到对应尺度的图片的坐标/////////sPt为scale//反归一化
			////////////////////////////////////////////

			double w = (weightsV[i])*std::exp(-((aPt - bPt).dot(aPt - bPt)) / 2) / std::sqrt(sPt.dot(Point3d(1, 1, 1)));
			//重新计算的权重，原权重weightsV[i]为线性SVM的得分
			resPoint += w*aPt;//根据中心点的距离加相应的权值,距离越近,权值越大

			ratPoint.x += w / sPt.x;//这边除以权重值，使得放缩后的图像权重变小
			ratPoint.y += w / sPt.y;
			ratPoint.z += w / sPt.z;

		}
		resPoint.x /= ratPoint.x;
		resPoint.y /= ratPoint.y;
		resPoint.z /= ratPoint.z;
		return resPoint;//返回被周围点影响后的值
	}

	double getResultWeight(const Point3d& inPt) const
	{
		double sumW = 0;//最终返回的值
		int num = 0;
		size_t aa = positionsV.size();//位置点的个数
		int len = int(aa);//位置点的个数
		for (size_t i = 0; i < aa; i++)
		{
			Point3d aPt = positionsV[i];
			Point3d sPt = densityKernel;

			sPt.x *= exp(aPt.z);
			sPt.y *= exp(aPt.z);

			aPt -= inPt;//讲个坐标之差

			aPt.x /= sPt.x;
			aPt.y /= sPt.y;
			aPt.z /= sPt.z;
			/*-----------------这块还可以优化，策略的考虑，权重的选取--begin-----------------*/
			if (aa > 10)//考虑如果aa的数量过小时  
			{
				double mm = aPt.dot(aPt);

				if (aPt.dot(aPt) <= 0.5)//两个点的欧式距离
				{

					if (weightsV[i] > 0.6)//weightsV[i] 为svm的权值
					{
						sumW += 0.35;
					}
					else if (weightsV[i] > 0.3)
					{
						sumW += 0.3;
					}
					continue;
				}
				//////////////////////////原始///////////
				sumW += (weightsV[i])*std::exp(-(aPt.dot(aPt)) / 2) / std::sqrt(sPt.dot(Point3d(1, 1, 1)));
				/////////////////////////////////////////
			}
			else
			{
				//sumW+=weightsV[i];
				sumW += (weightsV[i])*std::exp(-(aPt.dot(aPt)) / 2)*2.8;

			}
			/*---------待优化部分-----------------*/
			/*------这块还可以优化，策略的考虑，权重的选取--end------*/

			return sumW;//计算最后的权值
		}

		Point3d moveToMode(Point3d aPt, Rect inR, const vector<Rect>list) const
		{
			//均值漂移后的位置
			Point3d bPt;
			for (int i = 0; i < iterMax; i++)
			{
				bPt = aPt;
				aPt = getNewValue(bPt, inR, list);
				if (getDistance(aPt, bPt) <= modeEps)//小于阈值时返回，说明达到稳定状态
				{
					break;
				}
			}
			return aPt;//返回稳定状态的值
		}

		double getDistance(Point3d p1, Point3d p2) const
		{//计算欧式距离
			Point3d ns = densityKernel;
			ns.x *= exp(p2.z);
			ns.y *= exp(p2.z);
			p2 -= p1;
			p2.x /= ns.x;
			p2.y /= ns.y;
			p2.z /= ns.z;
			return p2.dot(p2);
		}
	};

	//被groupRectangles_meanshift1调用
	void groupRectangles_meanshift2(vector<Rect>& rectList, double detectThreshold, vector<double>* foundWeights,
		vector<double>& scales, Size winDetSize)
	{
		int detectionCount = (int)rectList.size();//矩形的个数
		vector<Point3d> hits(detectionCount), resultHits;
		vector<double> hitWeights(detectionCount), resultWeights;
		Point2d hitCenter;

		for (int i = 0; i < detectionCount; i++)
		{
			hitWeights[i] = (*foundWeights)[i];
			hitCenter = (rectList[i].tl() + rectList[i].br())*(0.5); //center of rectangles
			hits[i] = Point3d(hitCenter.x, hitCenter.y, std::log(scales[i]));//中心坐标x、y、log(scale)
		}


		if (foundWeights)
			foundWeights->clear();

		double logZ = std::log(1.3);
		Point3d smothing(8, 16, logZ);

		MeanshiftGrouping msGrouping(smothing, hits, rectList, hitWeights, 1e-5, 100);//得到
		///////////////////////////////////////////////////////////////////////
		msGrouping.getModes(resultHits, resultWeights, 1);
		//////////////////////////////////////////////////////////////////////

		rectList.clear();
		for (unsigned i = 0; i < resultHits.size(); ++i)
		{

			double scale = exp(resultHits[i].z);//还原尺度
			hitCenter.x = resultHits[i].x;//中心坐标
			hitCenter.y = resultHits[i].y;
			Size s(int(winDetSize.width*scale), int(winDetSize.height*scale));//还原窗的大小
			Rect resultRect(int(hitCenter.x - s.width / 2), int(hitCenter.y - s.height / 2),
				int(s.width), int(s.height));//还原窗的位置

			if (resultWeights[i] > detectThreshold)//detectThreshold 大于阈值的权重值输出
			{//返回矩形框和权值
				rectList.push_back(resultRect);
				foundWeights->push_back(resultWeights[i]);
			}
		}
	}

	void groupRectangles_meanshift1(vector<Rect>& rectList, vector<double>& foundWeights,
		vector<double>& foundScales, double detectThreshold, Size winDetSize)
	{
		groupRectangles_meanshift2(rectList, detectThreshold, &foundWeights, foundScales, winDetSize);
		//rectList矩形列表, detectThreshold阈值, foundWeights得分, foundScales尺度, winDetSize窗口大小
	}
}



int main(int argc,char*argv[])
{

	vector<Rect> found_filtered;

	for ()
	{
		/*----------------------------------分数判定：begin--------------------------------------------------*/
		//////////////将感兴趣区域归一化后计算HOG特征--begin////////////////////
		cvSetImageROI(imageOrigin, r);
		IplImage *test = cvCreateImage(cvGetSize(imageOrigin), 8, 3);
		cvCopyImage(imageOrigin, test);
		cvResetImageROI(imageOrigin);
		IplImage* testTempImg = cvCreateImage(cvSize(40, 40), 8, 3);
		cvZero(testTempImg);
		cvResize(test, testTempImg);
		hog->compute(testTempImg, descriptors, Size(1, 1), Size(0, 0)); //Hog特征计算
		Mat another(descriptors);
		descriptors.clear();
		////////////将感兴趣区域归一化后计算HOG特征--end///////////////
		
		double ret = 0;
		ret = ssvm.get_calc_liner(vec_count, var_count, a, another, results, alpha, 0) - rho; //计算SVM分数		
		////////////////////////////
		cvReleaseImage(&test);
		cvReleaseImage(&testTempImg);
		////////////////////////////
		if (ret < 0){
			continue;  //去掉SVM 分值小于0的图像
		}
	
		rc_.push_back(rcc);
		weights.push_back(ret);
		double rate = max(rcc.width*1.0 / 40, rcc.height*1.0 / 40);
		//选取长宽最大值，作为尺度，考虑还不够周到，这里改变，Meanshift 全部都要改
		//这里还可以改进
		foundScales.push_back(rate);
		/*----------------------------------分数判定：end--------------------------------------------------*/
	}
	//////////上述应该是一个循环，加入多个点//////////////




	/////////////////////////融合过程-begin////////////////////////////
	//vector<Rect> rc_;			   //矩形框
	//	vector<double> weights;    //权重，score
	//	vector<double> foundScales;//放缩尺度 
	groupRectangles_meanshift1(rc_, weights, foundScales, 0.3, Size(40, 40)); //框出来的矩形框进行融合
	//groupRectangles_meanshift1该函数在最后定义//
	///////////////////////融合过程-end///////////////////////////


	//////////////////重合去重第一步：计算融合后的分值//////////////////
	for (int i = 0; i < rc_.size(); i++)
	{
		Rect r = rc_[i];
		found_filtered.push_back(r);//加入矩形框
	}
	vector<float> found_score(found_filtered.size()); //矩形框的分数
	for (int i = 0; i < found_filtered.size(); i++)
	{
		Rect r = found_filtered[i];
		cvSetImageROI(imageOrigin, r);
		IplImage *test = cvCreateImage(cvGetSize(imageOrigin), 8, 3);
		cvCopyImage(imageOrigin, test);
		cvResetImageROI(imageOrigin);
		IplImage* testTempImg = cvCreateImage(cvSize(40, 40), 8, 3);
		cvZero(testTempImg);
		cvResize(test, testTempImg);
		hog->compute(testTempImg, descriptors, Size(1, 1), Size(0, 0)); //Hog特征计算
		Mat another(descriptors);
		descriptors.clear();
		double ret = 0;
		ret = ssvm.get_calc_liner(vec_count, var_count, a, another, results, alpha, 0) - rho;
		cvReleaseImage(&test);
		cvReleaseImage(&testTempImg);
		found_score[i] = ret;
		////////////////////////////
	}
	////////重合去重第二步：矩形框内的部分，取分值最大的/////
	//////////////////////found_filtered为融合后的矩形框//////////////////////////
	for (int i = 0; i < found_filtered.size(); i++)
	{
		//进行了重合去除，选取附近分值最大矩形框
		Rect r = found_filtered[i];
		for (int j = 0; j < rc_.size(); j++)

			if (j != i && (r & found_filtered[j]).area() == r.area())
			{//这里是将重叠的部分，分值小的矩形框的权重设为-1,为了取最大值
			if (found_score[i]>found_score[j])
			{
				found_score[j] = -1;
				break;
			}
			else
			{
				found_score[i] = -1;
				break;
			}
			}


	}
	////////重合去重第三步：重叠部分，取分值最大的/////
	//////////////////////////////////////////////////////////////////
	for (int i = 0; i < found_filtered.size(); i++)
	{
		//进行了重合去除
		Rect r = found_filtered[i];
		for (int j = 0; j < rc_.size(); j++)
			//判定重合是否大于相较面积的70%（这个比例有待测试）,判定都为上一步过滤后的结果，可能存在部分相较，但不包含的情况
			if (j != i && (r & found_filtered[j]).area() >= r.area()*0.7 &&found_score[j] != -1 && found_score[i] != -1)
			{//这里是将重叠的部分，分值小的矩形框的权重设为-1
				if (found_score[i]>found_score[j])
				{
					found_score[j] = -1;
					break;
				}
				else{
					found_score[i] = -1;
					break;
				}
			}


	}
	for (int i = 0; i < found_filtered.size(); i++)
	{
		if (found_score[i] == -1)//将分值等于-1的过滤掉
		{
			continue;
		}
		Rect r = found_filtered[i];
		r.x -= cvRound(r.width*0.05);
		r.width = cvRound(r.width*1.05);
		r.y -= cvRound(r.height*0.05);
		r.height = cvRound(r.height*1.05);
		rectangle(img_dst, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);	//在图像上画矩形框
	}

	return 0;
}

