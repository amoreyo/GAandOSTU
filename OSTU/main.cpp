// 大津算法又称作最大类间方差法
// m1 = 背景的像素均值， m2 = 主体的像素均值， mg = 图片的全局均值

# include<iostream>
# include<opencv2/core.hpp>
# include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2/highgui/highgui_c.h>

// cv::Mat 是存储数据的类，类似numpy
int Otsu(cv::Mat& src, cv::Mat& dst, int thresh)
{
	const int Grayscale = 256;
	int graynum[Grayscale] = { 0 };				// 初始化256的数组为0
	int r = src.rows;							// src的行数
	int c = src.cols;							// src的列数
	for (int i = 0; i < r; i++)
	{
		const uchar* ptr = src.ptr<uchar>(i);   // uchar是无符号字符，char的范围是-128~127，但是无符号的数值范围在0~255，在图像处理中表示像素值
												// cv::Mat .ptr 能返回指定行的地址
		for (int j = 0; j < c; j++)
		{
			graynum[ptr[j]]++;					// 统计src每个灰度值的直方图
		}
	}

	double P[Grayscale] = { 0 };
	double PK[Grayscale] = { 0 };
	double MK[Grayscale] = { 0 };
	double srcpixnum = r * c, sumtmpPK = 0, sumtmpMK = 0;
	for (int i = 0; i < Grayscale; i++)
	{
		P[i] = graynum[i] / srcpixnum;			// 统计每种灰度值出现的频率
		PK[i] = sumtmpPK + P[i];				// 概率累计和,相当于分布函数积分
		sumtmpPK = PK[i];
		MK[i] = sumtmpMK + i * P[i];            // 统计灰度均值
		sumtmpMK = MK[i];
	}

	// 计算类间方差
	double Var = 0;
	for (int k = 0; k < Grayscale; k++)
	{
		// 类间方差公式
		double leijian_Var = (MK[Grayscale - 1] * PK[k] - MK[k]) * (MK[Grayscale - 1] * PK[k] - MK[k]) / (PK[k] * (1 - PK[k]));
		if (leijian_Var > Var)
		{
			Var = leijian_Var;
			thresh = k;
		}
	}

	// 构建出已经完成分离的 cv::Mat
	src.copyTo(dst); // 把src拷贝给det咯
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
	// 如果数据为空
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

	// cv::namedWindow 新建窗口
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	cv::imshow("src", src);
	cv::namedWindow("dst", CV_WINDOW_NORMAL);
	cv::imshow("dst", dst);

	// 用户按任意键结束等待
	cv::waitKey(0);

	return 0;
}