#pragma once
# include<iostream>
# include<opencv2/core.hpp>
# include<opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <opencv2\imgproc\types_c.h>
# include <opencv2/highgui/highgui_c.h>

const int n = 4;
const int epoch = 6;
const int rand_add = 3;
const int all_node = 50;
// return thresh
int Otsu(cv::Mat& src, cv::Mat& dst, int thresh);

// return thresh[]
double OtsuV1(cv::Mat& src, int* thresh);