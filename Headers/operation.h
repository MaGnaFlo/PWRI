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
    void readFileName(std::string filePath, std::string &path, std::string &name, int &imgStart, std::string& ext);
    int string2int(const std::string imgStart);
    int findlastImage(std::string path, std::string name, std::string ext, int imgStart);

};

#endif // OPERATION_H
