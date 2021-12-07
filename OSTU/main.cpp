// ����㷨�ֳ��������䷽�
// m1 = ���������ؾ�ֵ�� m2 = ��������ؾ�ֵ�� mg = ͼƬ��ȫ�־�ֵ

# include<iostream>
# include<opencv2/core.hpp>
# include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2/highgui/highgui_c.h>

// cv::Mat �Ǵ洢���ݵ��࣬����numpy
int Otsu(cv::Mat& src, cv::Mat& dst, int thresh)
{
	const int Grayscale = 256;
	int graynum[Grayscale] = { 0 };				// ��ʼ��256������Ϊ0
	int r = src.rows;							// src������
	int c = src.cols;							// src������
	for (int i = 0; i < r; i++)
	{
		const uchar* ptr = src.ptr<uchar>(i);   // uchar���޷����ַ���char�ķ�Χ��-128~127�������޷��ŵ���ֵ��Χ��0~255����ͼ�����б�ʾ����ֵ
												// cv::Mat .ptr �ܷ���ָ���еĵ�ַ
		for (int j = 0; j < c; j++)
		{
			graynum[ptr[j]]++;					// ͳ��srcÿ���Ҷ�ֵ��ֱ��ͼ
		}
	}

	double P[Grayscale] = { 0 };
	double PK[Grayscale] = { 0 };
	double MK[Grayscale] = { 0 };
	double srcpixnum = r * c, sumtmpPK = 0, sumtmpMK = 0;
	for (int i = 0; i < Grayscale; i++)
	{
		P[i] = graynum[i] / srcpixnum;			// ͳ��ÿ�ֻҶ�ֵ���ֵ�Ƶ��
		PK[i] = sumtmpPK + P[i];				// �����ۼƺ�,�൱�ڷֲ���������
		sumtmpPK = PK[i];
		MK[i] = sumtmpMK + i * P[i];            // ͳ�ƻҶȾ�ֵ
		sumtmpMK = MK[i];
	}

	// ������䷽��
	double Var = 0;
	for (int k = 0; k < Grayscale; k++)
	{
		// ��䷽�ʽ
		double leijian_Var = (MK[Grayscale - 1] * PK[k] - MK[k]) * (MK[Grayscale - 1] * PK[k] - MK[k]) / (PK[k] * (1 - PK[k]));
		if (leijian_Var > Var)
		{
			Var = leijian_Var;
			thresh = k;
		}
	}

	// �������Ѿ���ɷ���� cv::Mat
	src.copyTo(dst); // ��src������det��
	for (int i = 0; i < r; i++)
	{
		uchar* ptr = dst.ptr<uchar>(i);
		for (int j = 0; j < c; j++)
		{
			if (ptr[j] > thresh)
				ptr[j] = 255;
			else
				ptr[j] = 0;
		}

	}

	return thresh;
}

int main()
{
	cv::Mat src = cv::imread("F:\git_repo\OSTU\0000-0000L_1002.png");
	// �������Ϊ��
	if (src.empty())
	{
		return -1;
	}
	if (src.channels() > 1)
	{
		cv::cvtColor(src, src, cv::ColorConversionCodes::COLOR_RGB2GRAY);
	}
	
	cv::Mat dst;
	int thresh = 0;
	double t2 = (double)cv::getTickCount();
	thresh = Otsu(src, dst, thresh);
	std::cout << "Mythresh = " << thresh << std::endl;
	t2 = (double)cv::getTickCount() - t2;
	double time2 = (t2 * 1000.) / ((double)cv::getTickFrequency());
	std::cout << "my_process=" << time2 << " ms. " << std::endl << std::endl;
	double  Otsu = 0;

	// cv::namedWindow �½�����
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	cv::imshow("src", src);
	cv::namedWindow("dst", CV_WINDOW_NORMAL);
	cv::imshow("dst", dst);

	// �û�������������ȴ�
	cv::waitKey(0);

	return 0;
}