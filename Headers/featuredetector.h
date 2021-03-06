/*!
 * \authors : Florian Lardeux, Thomas Maifret, Victor Petitjean
 * November 2015
 *
 * FeatureDetector uses OpenCV tools to compute key-points and matches
 * on pictures using SIFT.
*/

#ifndef FEATUREDETECTOR_H
#define FEATUREDETECTOR_H

#include <stdio.h>
#include <iostream>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "operation.h"
#include "assert.h"

class featureDetector
{
public:
    //Constructors
    featureDetector();
    featureDetector(int minHessian);

    //Methods
    void findKeypoints(std::string path, int oneOrTwo);
    void findMatches();
    void switch2to1();

    //Getters
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
