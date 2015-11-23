#ifndef FEATUREDETECTOR_H
#define FEATUREDETECTOR_H

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

class featureDetector
{
public:
    featureDetector();
    featureDetector(int minHessian);

    void findKeypoints(std::string path, int oneOrTwo);
    void findMatches();

    void switch2to1();

    std::vector<cv::DMatch> getSortedMatches() {return _matches;}
    std::vector<cv::KeyPoint> getKeyPoints(int oneOrTwo) {return (oneOrTwo == 1)?_keyPoint1:_keyPoint2;}

private:

    int _minHessian;
    cv::Ptr<cv::xfeatures2d::SURF> _detector;
    std::vector<cv::DMatch> _matches;
    std::vector<cv::KeyPoint> _keyPoint1;
    std::vector<cv::KeyPoint> _keyPoint2;
    cv::Mat _descriptor1;
    cv::Mat _descriptor2;

};

#endif // FEATUREDETECTOR_H
