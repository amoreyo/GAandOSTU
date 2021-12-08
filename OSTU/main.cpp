# include <iostream>
# include <stdlib.h>
# include <opencv2/core.hpp>
# include <opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <opencv2\imgproc\types_c.h>
# include <opencv2/highgui/highgui_c.h>
# include <time.h>
# include <algorithm>
# include "OSTU.h"

struct Node
{
	int rand_om[n + 1] = { 0 };
	double loss;
	bool operator >=(const Node& a)const
	{
		return loss >= a.loss;
	}
	bool cut = true;
};

Node node[100];

void comb_node(int j, int start_node)
{
	std::memset(node[start_node + j].rand_om, 0, sizeof(node[start_node + j].rand_om));
	node[start_node + j].rand_om[n] = 255;
	node[start_node + j].cut = false;
	for (int i = 1; i < n; i++)
	{
		node[start_node + j].rand_om[i] += node[j].rand_om[i];
		node[start_node + j].rand_om[i] += node[j+1].rand_om[i];
		node[start_node + j].rand_om[i] = int(node[start_node + j].rand_om[i] / 2);
	}
	
}

void rand_loss(cv::Mat src, int j)
{
	srand((unsigned)time(NULL));
	std::memset(node[j].rand_om, 0, sizeof(node[j].rand_om));
	node[j].rand_om[n] = 255;
	node[j].cut = false;
	for (int i = 1; i < n; i++)
	{
		node[j].rand_om[i] = (rand() % 254) + 1;
	}
	std::sort(node[j].rand_om, node[j].rand_om + n);
	node[j].loss = OtsuV1(src, node[j].rand_om);
	return;
}


// 降序
bool cmp(Node a, Node b)
{
	return a.loss > b.loss;
}

// class
int main()
{

	cv::Mat src = cv::imread("F:\\git_repo\\OSTU\\randm.png");
	// 如果数据为空
	if (src.empty())
	{
		return -1;
	}
	if (src.channels() > 1)
	{
		cv::cvtColor(src, src, cv::ColorConversionCodes::COLOR_RGB2GRAY);
	}


	// +4 /3
	// 初始化node
	for (int j = 0; j < all_node; j++)
	{
		rand_loss(src, j);
	}

	int start_node = all_node;
	for (int epo = 0; epo < epoch; epo++)
	{
		// add random
		for (int j = start_node; j < start_node + rand_add; j++)
		{
			rand_loss(src, j);
		}

		start_node = start_node + rand_add;
		// sort
		std::sort(node, node + start_node, cmp);
		// cut
		int cut_number = 2 * int(start_node / 3 + 0.5);
		for (int j = start_node - cut_number; j < start_node; j++)
		{
			node[j].cut = true;
		}
		start_node = start_node - cut_number;
		// 杂交组合. var 近亲杂交, 计算loss
		for (int j = 0; j < start_node - 1; j++)
		{
			comb_node(j, start_node);
			node[start_node + j].loss = OtsuV1(src, node[start_node + j].rand_om);
		}
		start_node = start_node * 2 - 1;
	}
	// epoch 结束之后

	cv::Mat dst;
	int r = src.rows;							
	int c = src.cols;							
	// 构建出已经完成分离的 cv::Mat
	src.copyTo(dst); // 把src拷贝给det咯
	for (int i = 0; i < r; i++)
	{
		uchar* ptr = dst.ptr<uchar>(i);
		for (int j = 0; j < c; j++)
		{
			if (ptr[j] < node[0].rand_om[1])
				ptr[j] = 255;
			else if (ptr[j] >= node[0].rand_om[1] && ptr[j] < node[0].rand_om[2])
				ptr[j] = 192;
			else if (ptr[j] >= node[0].rand_om[2] && ptr[j] < node[0].rand_om[3])
				ptr[j] = 128;
			else if (ptr[j] >= node[0].rand_om[3] && ptr[j] < node[0].rand_om[4])
				ptr[j] = 64;
			else if (ptr[j] >= node[0].rand_om[4] && ptr[j] <= 255)
				ptr[j] = 0;
		}
	}
	cv::namedWindow("srcwindow", CV_WINDOW_NORMAL);
	cv::imshow("src", src);
	cv::namedWindow("dstwindow", CV_WINDOW_NORMAL);
	cv::imshow("dst", dst);
	cv::waitKey();
	cv::imwrite("F:\\git_repo\\OSTU\\randm_dst.png", dst);
	// std::cout << "Mythresh = " << thresh << std::endl;
	//   t2 = (double)cv::getTickCount() - t2;
	//double time2 = (t2 * 1000.) / ((double)cv::getTickFrequency());
	//std::cout << "my_process=" << time2 << " ms. " << std::endl << std::endl;
	//double  Otsu = 0;

	// cv::namedWindow 新建窗口
	//cv::namedWindow("srcwindow", CV_WINDOW_AUTOSIZE);
	//cv::imshow("src", src);
	// cv::waitKey();
	//cv::namedWindow("dstwindow", CV_WINDOW_AUTOSIZE);
	//cv::imshow("dst", dst);
	//cv::waitKey();

	return 0;
}