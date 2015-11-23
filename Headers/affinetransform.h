#ifndef GEOMETRYTRANSFORM_H
#define GEOMETRYTRANSFORM_H

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "operation.h"

class affineTransform
{
public:
    affineTransform();
    affineTransform(int imgStart, int imgEnd, std::string path, std::string name, std::string ext);

    void setMidXY();

    cv::Mat createWarpedImages_rec(int i, std::vector<cv::KeyPoint> kpt1, std::vector<cv::KeyPoint> kpt2);
    void  createWarpedImages_it(int i, std::vector<cv::KeyPoint> kpt1, std::vector<cv::KeyPoint> kpt2);
    void findAffineTransform(std::vector<cv::DMatch> &matches, std::vector<cv::KeyPoint> &keypoint_1, std::vector<cv::KeyPoint> &keypoint_2);
    void process();
    void displayMatches(const std::vector<cv::DMatch> &matches, const std::vector<cv::KeyPoint> &keypoint_1, const std::vector<cv::KeyPoint> &keypoint_2, const cv::Mat &img1, const cv::Mat &img2, const std::string path);
    void createWarpedImages_it_mean(int j, std::vector<cv::KeyPoint> kpt1, std::vector<cv::KeyPoint> kpt2,cv::Mat mean);

private:
    Operation _op;
    std::vector<cv::Mat> _affVector;
    int _midX;
    int _midY;
    int _imgStart;
    int _imgEnd;
    std::string _path;
    std::string _name;
    std::string _ext;
    std::vector<cv::DMatch> _goodMatches;
};

#endif // GEOMETRYTRANSFORM_H
