#ifndef AFFINETRANSFORM_H
#define AFFINETRANSFORM_H

#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QProgressBar>

#include "operation.h"
#include "featuredetector.h"

class affineTransform
{
public:
    //Constructors
    affineTransform();
    affineTransform(int imgStart, int imgEnd, std::string path, std::string name, std::string ext);

    //Methods
    void setMidXY();
    cv::Mat createWarpedImages_rec(const int &i);
    void createWarpedImages_it();
    void createWarpedImages_it(const std::string &pathSave, QProgressBar *progressBar);
    void findAffineTransform(const std::vector<cv::DMatch> &matches, const std::vector<cv::KeyPoint> &keypoint_1, const std::vector<cv::KeyPoint> &keypoint_2);
    void process(QProgressBar *progressBar);
    void displayMatches(const std::vector<cv::DMatch> &matches, const std::vector<cv::KeyPoint> &keypoint_1, const std::vector<cv::KeyPoint> &keypoint_2, const cv::Mat &img1, const cv::Mat &img2, const std::string path);
    void displayFeatures(const std::vector<cv::KeyPoint> &kpt1, const std::vector<cv::KeyPoint> &kpt2, int i);

    //Setters
    void setMeanTransform(const cv::Mat &meanTransform);
    void setImgStart(const int &imgStart);
    void setImgEnd(const int &imgEnd);
    void setPath(const std::string &path);
    void setName(const std::string &name);
    void setExt(const std::string &ext);

    //Getters
    cv::Mat meanTransform() const;

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
    cv::Mat _meanTransform;

    inline cv::Mat mean(std::vector<cv::Mat> vect) const;
};

#endif // AFFINETRANSFORM_H
