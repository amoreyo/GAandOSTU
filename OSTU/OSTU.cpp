// ����㷨�ֳ��������䷽�
// m1 = ���������ؾ�ֵ�� m2 = ��������ؾ�ֵ�� mg = ͼƬ��ȫ�־�ֵ

# include<iostream>
# include<opencv2/core.hpp>
# include<opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <opencv2\imgproc\types_c.h>
# include <opencv2/highgui/highgui_c.h>
# include "OSTU.h"
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


double OtsuV1(cv::Mat& src, int *thresh)
{
	int array[n - 1] = { 0 };

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
		
		PK[i] = sumtmpPK + P[i];				// �����ۼƺ�,�൱�ڷֲ��������� <1
		sumtmpPK = PK[i];
		MK[i] = sumtmpMK + i * P[i];            // ͳ�ƻҶ�ֱ��ͼ��ֵ ����ֵ���Ҷ�ֵ��Ƶ��
		sumtmpMK = MK[i];						// �ۼƻҶȾ�ֵ�ĺ�  < ��( )
	}


	// ����ÿ����ľ�ֵ,���ڷ���
	double in_Var[n] = { 0 };
	double m_mean[n] = { 0 };
	double p_mean[n] = { 0 };

	double up_valude = 0;
	for (int i = 0; i < n; i++)
	{
		m_mean[i] = MK[thresh[i + 1]] - up_valude;
		up_valude = MK[thresh[i + 1]];
	}

	up_valude = 0;
	for (int i = 0; i < n; i++)
	{
		p_mean[i] = PK[thresh[i + 1]] - up_valude;
		up_valude = PK[thresh[i + 1]];
	}

	for (int i = 1; i <= n; i++)
	{
		// dont forget 255 j=thresh[i]
		for (int j = thresh[i - 1]; j < thresh[i]; j++)
		{
			in_Var[i - 1] += sqrt((P[j] * j - m_mean[i - 1]) * (P[j] * j - m_mean[i - 1]));
		}
		if (i == n)
			in_Var[i - 1] += sqrt((P[255] * 255 - m_mean[i - 1]) * (P[255] * 255 - m_mean[i - 1]));
	}

	// ������䷽��
	// 4������ 1 2 3 4.  [1,2] [1,3] [1,4] [2,3] [2,4] [3,4] 3+2+1 = 6
	const int comb = n * (n - 1) / 2;
	double betw_Var_comb[comb] = { 0 };
	int count = 0;
	// double leijian_Var = (MK[Grayscale - 1] * PK[k] - MK[k]) * (MK[Grayscale - 1] * PK[k] - MK[k]) / (PK[k] * (1 - PK[k]));
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			// thresh[i] 0,1,2,3,4
			// p1*p2*((m1-m2)^2)
			betw_Var_comb[count] = p_mean[i] * p_mean[j] * (m_mean[i] - m_mean[j]) * (m_mean[i] - m_mean[j]);
			// (MK[Grayscale - 1] * PK[k] - MK[k]) * (MK[Grayscale - 1] * PK[k] - MK[k]) / (PK[k] * (1 - PK[k]));
		}
	}

	double betw_Var[n] = { 0 };

	int start = 0;
	int end = n - 2;
	int m;
	for (int i = 0; i < n; i++)
	{
		for (int j = start; j <= end; j++)
		{
			betw_Var[i] += betw_Var_comb[j];
			betw_Var[i + 1 + j - start] += betw_Var_comb[j];
		}
		m = start;
		start = end + 1;
		end = 2 * end - m;
	}


	// ������䷽��ĺͳ������ڷ���
	double all_var = 0;
	for (int i = 0; i < n; i++)
	{
		all_var += betw_Var_comb[i] / m_mean[i];
	}

	return all_var;
}