
#include <stdio.h>
#include <time.h>
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
#include "videomanager.h"
#include "affinetransform.h"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

bool cmpfun(DMatch a, DMatch b) { return a.distance < b.distance; }
String int2string(int n);
Mat createWarpedImages(int i, vector<Mat> &transf);
void createWarpedImages2(int j, vector<Mat> transf);
Mat convert2x3to3x3(Mat transf2x3);


int main()
{
    clock_t start = clock();
    /*videoManager vm("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105",
                    "png",100,"Image 2015_11_04_180411,842 Image ", "lol");
    vm.createVideo();
    return 0;*/


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

    //Mat img1 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 000.png", IMREAD_GRAYSCALE);
    //Mat img2 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 010.png", IMREAD_GRAYSCALE);
    /*Mat img1 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/lee.jpg");
    Mat img2 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/lee2.jpg");

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

    ///Step 5 : Draw only good matches ---------------------------------------------------------------
    ///A good match has a distance of less than 2*min_dist,
    ///or a small arbitrary value (like 0.02) in the event that min is
    ///very small. radiusMatch can also be used.
    vector<DMatch> good_matches;
    Point2f coord1[3], coord2[3], coord3[3], coord4[3];;
    sort(matches.begin(), matches.end(), cmpfun);

    int seuil_x = 80;
    int seuil_y = 80;
    float seuil_l = 20;

    int i = 1;

    //First match is the best match
    good_matches.push_back(matches[0]);

    //Second match is the second best match that is far enough from the first match
    while(keypoint_1[matches[i].queryIdx].pt.x - keypoint_1[good_matches[0].queryIdx].pt.x < seuil_x
          || keypoint_1[matches[i].queryIdx].pt.y - keypoint_1[good_matches[0].queryIdx].pt.y < seuil_y)
    {
        i++;
    }
    good_matches.push_back(matches[i]);


    //Third match is the following best match which is far enough from both first and second matches.
    //the criterium chosen is that it should far enough from the line described by the first points

    Point2f A = keypoint_1[good_matches[0].queryIdx].pt;
    Point2f B = keypoint_1[good_matches[1].queryIdx].pt;
    Point2f C = keypoint_1[matches[i].queryIdx].pt;
    Vec2f d = B - A;
    float c = d(1)*A.x - d(0)*A.y;
    float l = abs(-d(1)*C.x + d(0)*C.y + c)/norm(d);

    while(l<seuil_l)
    {
        i++;
        C = keypoint_1[matches[i].queryIdx].pt;
        l = abs(-d(1)*C.x + d(0)*C.y + c)/norm(d);
    }
    good_matches.push_back(matches[i]);

    //Here we find the 3 best matches (6 points : 3 --> match1, 3 --> match2)
    for(int j = 0 ; j<3 ; j++)
    {
        Point2f pt1 = keypoint_1[good_matches[j].queryIdx].pt;
        Point2f pt2 = keypoint_2[good_matches[j].trainIdx].pt;
        coord1[j] = pt1;
        coord2[j] = pt2;
    }

    for(int j = 0 ; j<3 ; j++)
    {
        Point2f pt1 = keypoint_1[matches[j].queryIdx].pt;
        Point2f pt2 = keypoint_2[matches[j].trainIdx].pt;
        coord3[j] = pt1;
        coord4[j] = pt2;
    }


    ///Step X : Find the affine transformation matrix
    Mat img1w, img2w, img3w;
    Mat affineTrans1 = getAffineTransform(coord1, coord2);
    Mat affineTrans2 = getAffineTransform(coord3, coord4);
    Mat affineTrans3 = (affineTrans1 + affineTrans2)/2.;
    //Mat perspTrans = getPerspectiveTransform(coord1, coord2);
    warpAffine(img1, img1w, affineTrans1, img2.size());
    warpAffine(img1, img2w, affineTrans2, img2.size());
    warpAffine(img1, img3w, affineTrans3, img2.size());
    imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/lee3.png", img1w);
    imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/lee4.png", img2w);
    imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/lee5.png", img3w);


    ///Step 6 : Draw only good matches
    Mat img_matches;
    drawMatches(img1, keypoint_1, img2, keypoint_2,
                good_matches, img_matches, Scalar::all(-1),
                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    namedWindow("Good matches", CV_WINDOW_NORMAL);
    imshow("Good matches", img_matches);
    waitKey(0);
    return 0;*/
    //-------------------------------
    /*

    string number;
    int minHessian = 600;
    Ptr<SURF> detector = SURF::create(minHessian);
    vector<KeyPoint> keypoint_1, keypoint_2;
    Mat descriptors_1, descriptors_2;
    BFMatcher matcher;
    vector<Mat> affTrans;
    Mat img1, img2;
    int n_images = 99;


    cout << "Processing first image" <<endl;

    img1 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 000.png", IMREAD_GRAYSCALE);
    img2 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 001.png", IMREAD_GRAYSCALE);

    if( !img1.data || !img2.data )
      { cout<< " --(!) Error reading images " << endl; return -1; }

    detector->detect(img1, keypoint_1);
    detector->detect(img2, keypoint_2);

    detector->compute(img1, keypoint_1, descriptors_1);
    detector->compute(img2, keypoint_2, descriptors_2);



    for(int i = 0 ; i<n_images ; i++)
    {
        cout << "Processing image " << i << endl;

        number = int2string(i);
        img2 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + number + ".png", IMREAD_GRAYSCALE);

        if( !img1.data || !img2.data )
          { cout<< " --(!) Error reading images " << endl; return -1; }

        ///Step 1 : Detect the keypoints using SURF detector
        keypoint_1 = keypoint_2;
        detector->detect(img2, keypoint_2);

        ///Step 2 : Calculate descriptors (feature vectors)
        descriptors_1 = descriptors_2;
        detector->compute(img2, keypoint_2, descriptors_2);


        ///Step 3 : Matching descriptor vectors using BFMatcher
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

        ///Step 5 : Draw only good matches ---------------------------------------------------------------
        ///A good match has a distance of less than 2*min_dist,
        ///or a small arbitrary value (like 0.02) in the event that min is
        ///very small. radiusMatch can also be used.
        vector<DMatch> good_matches;
        Point2f coord1[3], coord2[3];
        sort(matches.begin(), matches.end(), cmpfun);

        unsigned int i;

        int seuil_x = 80;
        int seuil_y = 80;
        float seuil_l = 50;

        bool reduce_l = true;
        bool reduce_x = false;
        bool reduce_y = false;

        while(true) {
            good_matches.clear();

            i = 1;

            //First match is the best match
            good_matches.push_back(matches[0]);
            std::cout << "First match ok" << endl;

            //Second match is the second best match that is far enough from the first match
            while(keypoint_1[matches[i].queryIdx].pt.x - keypoint_1[good_matches[0].queryIdx].pt.x < seuil_x
                  || keypoint_1[matches[i].queryIdx].pt.y - keypoint_1[good_matches[0].queryIdx].pt.y < seuil_y)
            {
                if(i<matches.size())
                {i++; cout <<i<<endl;}
                if(i==matches.size())
                {cout << "break"<< endl; break;}
            }

            if(i<matches.size())
                good_matches.push_back(matches[i]);

            std::cout << "snd match ok" << endl;

            //Third match is the following best match which is far enough from both first and second matches.
            //the criterium chosen is that it should far enough from the line described by the first points


            i++;
            if(i<matches.size()) {
                Point2f A = keypoint_1[good_matches[0].queryIdx].pt;
                Point2f B = keypoint_1[good_matches[1].queryIdx].pt;
                Point2f C = keypoint_1[matches[i].queryIdx].pt;
                Vec2f d = B - A;
                float c = d(1)*A.x - d(0)*A.y;
                float l = abs(-d(1)*C.x + d(0)*C.y + c)/norm(d);

                while(l<seuil_l && i<matches.size())
                {
                    C = keypoint_1[matches[i].queryIdx].pt;
                    l = abs(-d(1)*C.x + d(0)*C.y + c)/norm(d);
                    i++;
                }
            }

            if(i<matches.size())
                good_matches.push_back(matches[i]);
            std::cout << "Third match ok or bypassed" << endl;

            if(good_matches.size() == 3)
                break;
            else {
                if(reduce_l) {
                    seuil_l -= 10;
                    reduce_l = false;
                    reduce_y = true;
                }
                else if(reduce_l) {
                    seuil_l -= 10;
                    reduce_l = false;
                    reduce_y = true;
                }
                else if(reduce_y) {
                    seuil_y -= 10;
                    reduce_y = false;
                    reduce_x = true;
                }
                else if(reduce_x) {
                    seuil_x -= 10;
                    reduce_x = false;
                    reduce_l = true;
                }
            }
        }

        //Here we find the 3 best matches (6 points : 3 --> match1, 3 --> match2)
        for(int j = 0 ; j<3 ; j++)
        {
            Point2f pt1 = keypoint_1[good_matches[j].queryIdx].pt;
            Point2f pt2 = keypoint_2[good_matches[j].trainIdx].pt;
            coord1[j] = pt1;
            coord2[j] = pt2;
            cout << "point 1 " << pt1 <<endl;
            cout << "point 2 " << pt2 << endl;
        }
        std::cout << "Points set" << endl;

        ///Step X : Find the affine transformation matrix
        Mat affineTrans = getAffineTransform(coord2, coord1);
        std::cout << "Transform ok" << endl;
        affTrans.push_back(affineTrans);
        std::cout << affineTrans << endl;
    }
    cout << (double)(clock() - start)/CLOCKS_PER_SEC << endl;
    cout << "Processing transformation" << endl;
    //Mat mat = createWarpedImages(2, affTrans);
    for(int j = 0 ; j < n_images ; j++)
       { createWarpedImages2(j, affTrans); cout << j << endl; }

    cout << "Creating video" << endl;
    videoManager vm("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res",
                         "png",n_images,"","test");
    vm.createVideo();
    return 0;
     */

    /* string number;
     int minHessian = 600;
     Ptr<SURF> detector = SURF::create(minHessian);
     vector<KeyPoint> keypoint_1, keypoint_2;
     Mat descriptors_1, descriptors_2;
     BFMatcher matcher;
     vector<Mat> affTrans;
     Mat img1, img2;
     int mid_x, mid_y;
     int image_debut = 0;
     int image_fin_exclu = 5;


     cout << "Processing first image" <<endl;

     img1 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 000.png", IMREAD_GRAYSCALE);
     img2 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 001.png", IMREAD_GRAYSCALE);

     mid_x = img1.size().width / 2;
     mid_y = img1.size().height / 2;

     if( !img1.data || !img2.data )
       { cout<< " --(!) Error reading images " << endl; return -1; }

     detector->detect(img1, keypoint_1);
     detector->detect(img2, keypoint_2);

     detector->compute(img1, keypoint_1, descriptors_1);
     detector->compute(img2, keypoint_2, descriptors_2);



     for(int i = image_debut ; i<image_fin_exclu ; i++)
     {
         cout << "Processing image " << i << endl;

         number = int2string(i);
         img2 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + number + ".png", IMREAD_GRAYSCALE);

         if( !img1.data || !img2.data )
           { cout<< " --(!) Error reading images " << endl; return -1; }

         ///Step 1 : Detect the keypoints using SURF detector
         keypoint_1 = keypoint_2;
         detector->detect(img2, keypoint_2);

         ///Step 2 : Calculate descriptors (feature vectors)
         descriptors_1 = descriptors_2;
         detector->compute(img2, keypoint_2, descriptors_2);


         ///Step 3 : Matching descriptor vectors using BFMatcher
         vector<DMatch> matches;
         matcher.match(descriptors_1, descriptors_2, matches);

         ///Step 4 : Draw only good matches ---------------------------------------------------------------
         ///A good match has a distance of less than 2*min_dist,
         ///or a small arbitrary value (like 0.02) in the event that min is
         ///very small. radiusMatch can also be used.
         vector<DMatch> good_matches;
         Point2f coord1[3], coord2[3];
         sort(matches.begin(), matches.end(), cmpfun);

         unsigned int j = 0;

         int seuil = 200;

         while(true) {
             good_matches.clear();

             while(keypoint_1[matches[j].queryIdx].pt.x > mid_x-seuil
                   || keypoint_1[matches[j].queryIdx].pt.y > mid_y-seuil)
             {
                   j++;
                   if(j>=matches.size())
                   {
                       cout << "on va trop loin pour le 1st match" << endl;
                       break;
                   }
             }

             if(j<matches.size())
                 good_matches.push_back(matches[j]);

             cout << "1st match ok" << endl;

             j = 0;

             while(keypoint_1[matches[j].queryIdx].pt.x < mid_x+seuil
                   || keypoint_1[matches[j].queryIdx].pt.y > mid_y-seuil)
             {
                   j++;
                   if(j>=matches.size())
                   {
                       cout << "on va trop loin pour le 2nd match" << endl;
                       break;
                   }
             }

             if(j<matches.size())
                 good_matches.push_back(matches[j]);

             cout << "2nd match ok" << endl;

             j = 0;

             while(keypoint_1[matches[j].queryIdx].pt.y < mid_y+seuil)
             {
                   j++;
                   if(j>=matches.size())
                   {
                       cout << "on va trop loin pour le 3rd match" << endl;
                       break;
                   }
             }

             if(j<matches.size())
                 good_matches.push_back(matches[j]);

             cout << "3rd match ok" << endl;
             cout << seuil << endl;

             if(good_matches.size() == 3)
                 break;
             else {
                 seuil -= 10;
             }
         }

         //Here we find the 3 best matches (6 points : 3 --> match1, 3 --> match2)
         for(int j = 0 ; j<3 ; j++)
         {
             Point2f pt1 = keypoint_1[good_matches[j].queryIdx].pt;
             Point2f pt2 = keypoint_2[good_matches[j].trainIdx].pt;
             coord1[j] = pt1;
             coord2[j] = pt2;
             cout << "point 1 " << pt1 <<endl;
             cout << "point 2 " << pt2 << endl;
         }
         std::cout << "Points set" << endl;

         ///Step X : Find the affine transformation matrix
         Mat affineTrans = getAffineTransform(coord2, coord1);
         std::cout << "Transform ok" << endl;
         affTrans.push_back(affineTrans);
         std::cout << affineTrans << endl;
     }
     cout << (double)(clock() - start)/CLOCKS_PER_SEC << endl;

     cout << "Processing transformation" << endl;
     //Mat mat = createWarpedImages(2, affTrans);
     for(int j = 0 ; j < image_fin_exclu - image_debut ; j++)
        { createWarpedImages2(j, affTrans); cout << j << endl; }

*/

    affineTransform aff(0, 99 , "C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/", "Image 2015_11_04_180411,842 Image ", ".png");
    aff.process();
    cout << "Creating video" << endl;
    videoManager vm("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res",
                          "png",99,"","vid");
    videoManager vm2("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res",
                          "png",99,"mean","meanvid");
    vm.createVideo();
    vm2.createVideo();

     return 0;
}

Mat createWarpedImages(int i, vector<Mat> &transf) {

    if(i == 0) {
        Mat imgi = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 000.png", IMREAD_GRAYSCALE);
        imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/000.png", imgi);
        return Mat::eye(3,3,CV_64F);
    }
    else {
        Mat transf3x3 = Mat::zeros(3,3,CV_64F);
        for(int m = 0 ; m<2 ; m++)
            for(int n = 0 ; n<3 ; n++)
            transf3x3.at<double>(m,n) = transf[i].at<double>(m,n);

        transf3x3.at<double>(2,2) = 1.f;

        Mat res = transf3x3 * createWarpedImages(i - 1, transf);

        Mat res2x3 = res.rowRange(0,2) / res.at<double>(2,2);
        std::cout << res2x3 << endl;

        string number = int2string(i);
        Mat imgi = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + number + ".png", IMREAD_GRAYSCALE);
        Mat img_res;

        warpAffine(imgi, img_res, res2x3, imgi.size());
        imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/" + number + ".png", img_res);


        return res;

    }

}
    void createWarpedImages2(int j, vector<Mat> transf) {
        Mat trans_product = Mat::eye(3,3,CV_64F);
        for(int i = 0 ; i<j ; i++) {
            Mat t_3x3 = convert2x3to3x3(transf[i]);
            trans_product = trans_product * t_3x3;
        }

        Mat trans_product_2x3 = trans_product.rowRange(0,2) / trans_product.at<double>(2,2);

        string number = int2string(j);
        Mat imgi = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + number + ".png", IMREAD_GRAYSCALE);
        Mat img_warped;

        warpAffine(imgi, img_warped, trans_product_2x3, imgi.size());
        imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/" + number + ".png", img_warped);
    }

    Mat convert2x3to3x3(Mat transf2x3) {
        Mat transf3x3 = Mat::zeros(3,3,CV_64F);
        for(int m = 0 ; m<2 ; m++)
            for(int n = 0 ; n<3 ; n++)
                transf3x3.at<double>(m,n) = transf2x3.at<double>(m,n);

        transf3x3.at<double>(2,2) = 1.f;

        return transf3x3;
    }

    String int2string(int n) {
        string number;
        ostringstream convert;
        convert << n;
        if(n<10)
            number = "00" + convert.str();
        else if(n<100)
            number = "0" + convert.str();
        else
            number = convert.str();

        return number;
    }
