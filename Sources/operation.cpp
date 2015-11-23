#include "operation.h"
#include <iostream>

Operation::Operation()
{
}

std::string Operation::int2string(int n)
{
    std::string number;
    std::ostringstream convert;
    convert << n;
    if(n<10)
        number = "00" + convert.str();
    else if(n<100)
        number = "0" + convert.str();
    else
        number = convert.str();

    return number;
}

cv::Mat Operation::convert2x3to3x3(cv::Mat transf2x3)
{
    cv::Mat transf3x3 = cv::Mat::zeros(3,3,CV_64F);
    for(int m = 0 ; m<2 ; m++)
        for(int n = 0 ; n<3 ; n++)
            transf3x3.at<double>(m,n) = transf2x3.at<double>(m,n);

    transf3x3.at<double>(2,2) = 1.f;

    return transf3x3;
}

cv::Mat Operation::meanMat(const std::vector<cv::Mat> matrices)
{
    assert(matrices.size() != 0);

    cv::Mat mean = matrices.at(0) / (double) matrices.size();
    for(int i = 1 ; i<matrices.size() ; i++)
        mean += matrices.at(i) / (double) matrices.size();

    return mean;
}

