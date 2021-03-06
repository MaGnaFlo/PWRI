/*!
 * \authors : Florian Lardeux, Thomas Maifret, Victor Petitjean
 * November 2015
 *
 * FeatureDetector uses OpenCV tools to compute key-points and matches
 * on pictures using SIFT.
*/

#include "featuredetector.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;


/*!
 * matchCmp is a comparison function between two matches (DMatch in OpenCV)
*/

bool matchCmp(DMatch a, DMatch b) { return a.distance < b.distance; }


///// CONSTRUCTORS /////

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


///// METHODS /////

/*!
 * findKeyPoints computes one image, and saves the keypoints and the descriptors
 * for that image. It uses OpenCV methods.
 * We can choose to save the results in the 1st or 2nd slot
 * of _keyPoint{i} and _descriptor{i}
*/

void featureDetector::findKeypoints(string path, int oneOrTwo)
{
    Mat img;
    img = imread(path, IMREAD_GRAYSCALE);
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

/*!
 * findMatches is called once keypoints and descriptors have been computed,
 * and it saves matches of keypoints between two consecutive images.
*/

void featureDetector::findMatches()
{
    BFMatcher matcher;
    matcher.match(_descriptor1, _descriptor2, _matches);
    sort(_matches.begin(), _matches.end(), matchCmp);
}

/*!
 * switch2to1 can switch _keyPoint2 and _descriptor2 to the 1st slot.
 * Thanks to it, less images have to be open : it is then faster.
*/

void featureDetector::switch2to1()
{
    _descriptor1 = _descriptor2;
    _keyPoint1 = _keyPoint2;
}
