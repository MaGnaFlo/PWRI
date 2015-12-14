#ifndef OPERATION_H
#define OPERATION_H

#include <stdio.h>
#include <iostream>
#include <istream>
#include <sys/stat.h>
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>


class Operation
{
public:
    //Constructors
    Operation();

    std::string int2string(int n);
    cv::Mat convert2x3to3x3(cv::Mat transf2x3);
    cv::Mat meanMat(const std::vector<cv::Mat> matrices);
    void readFileName(std::string filePath, std::string &path, std::string &name, int &imgStart, std::string& ext, bool withNumber);
    int string2int(const std::string imgStart);
    int findlastImage(std::string path, std::string name, std::string ext, int imgStart);
    void minAndMaxVectorDouble(const std::vector<double> &vec, double &m, double &M);
};

#endif // OPERATION_H
