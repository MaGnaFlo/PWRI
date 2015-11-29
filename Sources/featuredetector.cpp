#include "featuredetector.h"
#include "operation.h"
#include "assert.h"
#include <algorithm>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

bool matchCmp(DMatch a, DMatch b) { return a.distance < b.distance; }


featureDetector::featureDetector()
{
    _minHessian = 600;
    _detector = SURF::create(_minHessian);

}

featureDetector::featureDetector(int minHessian)
{
    _minHessian = minHessian;
    _detector = SURF::create(_minHessian);
}

void featureDetector::findKeypoints(string path, int oneOrTwo)
{
    Mat img;
    img = imread(path, IMREAD_GRAYSCALE);
    std::cout << path << std::endl;

    assert(img.data);

    switch(oneOrTwo) {
    case 1:
        _detector->detect(img, _keyPoint1);
        _detector->compute(img, _keyPoint1, _descriptor1);
    break;

    case 2:
        _detector->detect(img, _keyPoint2);
        _detector->compute(img, _keyPoint2, _descriptor2);
    break;
    }

}


void featureDetector::findMatches()
{
    BFMatcher matcher;
    matcher.match(_descriptor1, _descriptor2, _matches);
    sort(_matches.begin(), _matches.end(), matchCmp);
}

void featureDetector::switch2to1()
{
    _descriptor1 = _descriptor2;
    _keyPoint1 = _keyPoint2;
}
