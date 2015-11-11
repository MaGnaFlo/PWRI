
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv_modules.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

bool cmpfun(DMatch a, DMatch b) { return a.distance < b.distance; }

int main()
{

    /*for(;;) {
        VideoCapture cap("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/video2.avi");
        if(!cap.isOpened())
            return -1;

        namedWindow("Video",WINDOW_FULLSCREEN);
        //setWindowProperty("Video", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);

        for(int i = 0 ; i<50 ; i++)
        {
            InputArray M = cv::Matx23f((double)  i/10,0,0,0,1,0);
            Mat frame;
            cap >> frame;
            cvtColor(frame, frame, COLOR_BGR2GRAY);
            warpAffine(frame,frame,M,frame.size());
            imshow("Video", frame);
            if(waitKey(30) >= 0)
                break;
        }

        break;
    }*/

    Mat img1 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 000.png", IMREAD_GRAYSCALE);
    Mat img2 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 001.png", IMREAD_GRAYSCALE);


    if( !img1.data || !img2.data )
      { cout<< " --(!) Error reading images " << endl; return -1; }

    ///Step 1 : Detect the keypoints using SURF detector
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create(minHessian);
    vector<KeyPoint> keypoint_1, keypoint_2;

    detector->detect(img1, keypoint_1);
    detector->detect(img2, keypoint_2);

    ///Step 2 : Calculate descriptors (feature vectors)
    Mat descriptors_1, descriptors_2;
    detector->compute(img1, keypoint_1, descriptors_1);
    detector->compute(img2, keypoint_2, descriptors_2);

    ///Step 3 : Matching descriptor vectors using BFMatcher
    BFMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_1, descriptors_2, matches);

    ///Step 4 : Calculate max and min distances between keypoints
    double max_dist = 0; double min_dist = 0;
    for(int i = 0; i<descriptors_1.rows; i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }

    ///Step 5 : Draw only good matches
    ///A good match has a distance of less than 2*min_dist,
    ///or a small arbitrary value (like 0.02) in the event that min is
    ///very small. radiusMatch can also be used.
    vector<DMatch> good_matches;
    vector<Point2f> match1, match2;
    sort(matches.begin(), matches.end(), cmpfun);

    for(int i = 0; i < 3; i++)
    {
        good_matches.push_back(matches[i]);

        //Here we find the 3 best matches (6 points : 3 --> match1, 3 --> match2)
        Point2f pt1 = keypoint_1[matches[i].queryIdx].pt;
        Point2f pt2 = keypoint_2[matches[i].trainIdx].pt;
        match1.push_back(pt1);
        match2.push_back(pt2);
    }

    ///Step X : Find the affine transformation matrix



    ///Step 6 : Draw only good matches
    Mat img_matches;
    drawMatches(img1, keypoint_1, img2, keypoint_2,
                good_matches, img_matches, Scalar::all(-1),
                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    namedWindow("Good matches", CV_WINDOW_NORMAL);
    imshow("Good matches", img_matches);
    waitKey(0);
    return 0;

}
