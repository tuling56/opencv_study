#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

void buildRandomForestClassifier(const char* _dataFileName);

void main1(void)
{
	printf("\nThis is letter recognition sample.\n"
		"classifier is Random Trees\n" );
	
	const char* dataFileName = "source\\randomforest\\letter-recognition.data";
	buildRandomForestClassifier(dataFileName);
	getchar();
}

void buildRandomForestClassifier(const char* _dataFileName)
{
	// ¶ÁÈ¡Êý¾Ý
	const int MAX_FEATURE = 100;
	const int VAR_COUNT = 16;
	const int NSAMPLES_ALL = 20000;
	
	FILE* f = fopen(_dataFileName, "rt");

	char buf[MAX_FEATURE+2];
	float* el_ptr;

	Mat data(NSAMPLES_ALL, 16, CV_32F);
	Mat responses(NSAMPLES_ALL, 1, CV_32F);

	el_ptr = new float[VAR_COUNT+1];

	for(int i=0; i<NSAMPLES_ALL; i++)
	{
		if (!fgets(buf, MAX_FEATURE, f) || !strchr(buf, ','))
			break;

		el_ptr[0] = buf[0];
		responses.at<float>(i,0) = el_ptr[0];
		char* ptr;
		ptr = buf+2;
		for (int j=1; j<=VAR_COUNT; j++)
		{
			int n = 0;
			sscanf(ptr, "%f%n", el_ptr + j, &n);
			ptr += n + 1;
			data.at<float>(i, j-1) = el_ptr[j];
		}
	}
	fclose(f);
	delete el_ptr;

	// ÑµÁ·Ëæ»úÉ­ÁÖ
	int ntrain_samples = (int)(NSAMPLES_ALL * 0.8);
	int i = 0;
	CvRTrees forest;

	forest.train(data, CV_ROW_SAMPLE, responses, Mat(), Mat(), Mat(), Mat(),
		CvRTParams(10,10,0,false,15,0,true,4,100,0.01f,CV_TERMCRIT_ITER));

	// Ô¤²âÎó²î
	double train_hr = 0;
	double test_hr = 0;

	for (int i=0; i<NSAMPLES_ALL; i++)
	{
		double r;
		Mat sample = data.row(i);

		r = forest.predict(sample);
		r = fabs((double)r - responses.at<float>(i,0)) <= FLT_EPSILON ? 1 : 0;

		if (i < ntrain_samples)
			train_hr += r;
		else
			test_hr += r;
	}

	test_hr /= (double)(NSAMPLES_ALL - ntrain_samples);
	train_hr /= (double)ntrain_samples;
	printf( "Recognition rate: train = %.1f%%, test = %.1f%%\n",
		train_hr*100., test_hr*100. );

	printf( "Number of trees: %d\n", forest.get_tree_count() );
}