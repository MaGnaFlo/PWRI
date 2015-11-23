#ifndef OPERATION_H
#define OPERATION_H

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>

class Operation
{
public:
    Operation();

    std::string int2string(int n);
    cv::Mat convert2x3to3x3(cv::Mat transf2x3);
    cv::Mat meanMat(const std::vector<cv::Mat> matrices);



};

#endif // OPERATION_H
