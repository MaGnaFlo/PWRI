#include "affinetransform.h"
#include "operation.h"
#include "featuredetector.h"

using namespace std;
using namespace cv;

affineTransform::affineTransform()
{
    setMidXY();
}

affineTransform::affineTransform(int imgStart, int imgEnd, string path, string name, string ext)
{
    _imgEnd = imgEnd;
    _imgStart = imgStart;
    _path = path;
    _name = name;
    _ext = ext;

    setMidXY();
}

void affineTransform::setMidXY()
{
    Mat im = imread(_path + _name + _op.int2string(_imgStart) + _ext);
    _midX = im.size().width / 2;
    _midY = im.size().height / 2;
    cout << "midX = " << _midX << " - midY = " << _midY << endl;
}


cv::Mat affineTransform::createWarpedImages_rec(int i, vector<KeyPoint> kpt1, vector<KeyPoint> kpt2)
{
    if(i == 0) {
        cout << "first" <<endl;
        Mat imgi = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image 000.png", IMREAD_GRAYSCALE);
        imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/000.png", imgi);
        return Mat::eye(3,3,CV_64F);
    }
    else {
        cout << _affVector.size() << endl;
        Mat transf3x3 = Mat::zeros(3,3,CV_64F);
        for(int m = 0 ; m<2 ; m++)
            for(int n = 0 ; n<3 ; n++)
                transf3x3.at<double>(m,n) = _affVector[i].at<double>(m,n);

        transf3x3.at<double>(2,2) = 1.f;

        Mat res = transf3x3 * createWarpedImages_rec(i - 1, kpt1, kpt2);

        Mat res2x3 = res.rowRange(0,2) / res.at<double>(2,2);

        string number = _op.int2string(i);
        Mat imgi = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + number + ".png", IMREAD_GRAYSCALE);
        Mat img_res;

        warpAffine(imgi, img_res, res2x3, imgi.size());
        imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/" + number + ".png", img_res);

        //---
        if(i+1<_imgEnd)
        {
            Mat imgi1 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + _op.int2string(i+1) + ".png", IMREAD_GRAYSCALE);
            displayMatches(_goodMatches, kpt1, kpt2, imgi, imgi1, "C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/features/" + number + ".png");
        }
            return res;
    }
}

void affineTransform::createWarpedImages_it(int j, vector<KeyPoint> kpt1, vector<KeyPoint> kpt2)
{
    Mat trans_product = Mat::eye(3,3,CV_64F);
    for(int i = 0 ; i<j ; i++) {
        Mat t_3x3 = _op.convert2x3to3x3(_affVector[i]);
        trans_product = trans_product * t_3x3;
    }

    Mat trans_product_2x3 = trans_product.rowRange(0,2) / trans_product.at<double>(2,2);

    string number = _op.int2string(j);
    Mat imgi = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + number + ".png", IMREAD_GRAYSCALE);
    Mat img_warped;

    warpAffine(imgi, img_warped, trans_product_2x3, imgi.size());
    imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/" + number + ".png", img_warped);

    //----
    if(j+1<_imgEnd)
    {
        Mat imgi1 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + _op.int2string(j+1) + ".png", IMREAD_GRAYSCALE);
        displayMatches(_goodMatches, kpt1, kpt2, imgi, imgi1, "C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/features/" + number + ".png");
    }
}

void affineTransform::createWarpedImages_it_mean(int j, vector<KeyPoint> kpt1, vector<KeyPoint> kpt2, Mat mean)
{
    Mat trans_product = Mat::eye(3,3,CV_64F);
    Mat t_3x3 = _op.convert2x3to3x3(mean);
    for(int i = 0 ; i<j ; i++) {
        trans_product = trans_product * t_3x3;
    }

    Mat trans_product_2x3 = trans_product.rowRange(0,2) / trans_product.at<double>(2,2);

    string number = _op.int2string(j);
    Mat imgi = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + number + ".png", IMREAD_GRAYSCALE);
    Mat img_warped;

    warpAffine(imgi, img_warped, trans_product_2x3, imgi.size());
    imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/mean" + number + ".png", img_warped);

    //----
    /*if(j+1<_imgEnd)
    {
        Mat imgi1 = imread("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105/Image 2015_11_04_180411,842 Image " + _op.int2string(j+1) + ".png", IMREAD_GRAYSCALE);
        displayMatches(_goodMatches, kpt1, kpt2, imgi, imgi1, "C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/features/" + number + ".png");
    }*/
}

void affineTransform::findAffineTransform(std::vector<DMatch> &matches, std::vector<KeyPoint> &keypoint_1, std::vector<KeyPoint> &keypoint_2)
{

    ///Step 4 : Draw only good matches ---------------------------------------------------------------
    ///A good match has a distance of less than 2*min_dist,
    ///or a small arbitrary value (like 0.02) in the event that min is
    ///very small. radiusMatch can also be used.
    ///

    vector<DMatch> good_matches;
    Point2f coord1[3], coord2[3];
    unsigned int j = 0;
    int seuil = 200;

    while(true) {
        good_matches.clear();
        j = 0;

        while(keypoint_1[matches[j].queryIdx].pt.x > _midX-seuil
              || keypoint_1[matches[j].queryIdx].pt.y > _midY-seuil)
        {
              j++;
              if(j>=matches.size())
              {
                  cout << "on va trop loin pour le 1st match" << endl;
                  break;
              }
        }

        if(j<matches.size())
        {
            good_matches.push_back(matches[j]);
            cout << "1st match ok" << endl;
        }

        j = 0;
        while(keypoint_1[matches[j].queryIdx].pt.x < _midX+seuil
              || keypoint_1[matches[j].queryIdx].pt.y > _midY-seuil)
        {
              j++;
              if(j>=matches.size())
              {
                  cout << "on va trop loin pour le 2nd match" << endl;
                  break;
              }
        }

        if(j<matches.size())
        {
            good_matches.push_back(matches[j]);
            cout << "2nd match ok" << endl;
        }

        j = 0;
        while(keypoint_1[matches[j].queryIdx].pt.y < _midY+seuil)
        {
              j++;
              if(j>=matches.size())
              {
                  cout << "on va trop loin pour le 3rd match" << endl;
                  break;
              }
        }

        if(j<matches.size())
        {
            good_matches.push_back(matches[j]);
            cout << "3rd match ok" << endl;
        }
        cout << "seuil = " << seuil << endl;

        if(good_matches.size() == 3)
            break;
        else {
            seuil -= 10;
        }
    }

    _goodMatches = good_matches;

    //Here we find the 3 best matches (6 points : 3 --> match1, 3 --> match2)
    for(int j = 0 ; j<3 ; j++)
    {
        Point2f pt1 = keypoint_1[_goodMatches[j].queryIdx].pt;
        Point2f pt2 = keypoint_2[_goodMatches[j].trainIdx].pt;
        coord1[j] = pt1;
        coord2[j] = pt2;
    }

    std::cout << "Points set" << endl;
    ///Step X : Find the affine transformation matrix
    Mat affineTrans = getAffineTransform(coord2, coord1);
    std::cout << "Transform ok" << endl;
    _affVector.push_back(affineTrans);
    std::cout << affineTrans << endl;

}

void affineTransform::process()
{
    featureDetector featDet;
    vector<DMatch> matches;
    vector<KeyPoint> kpt1, kpt2;

    _affVector.clear();

    for(int i = _imgStart ; i<_imgEnd-1 ; i++) // On risque de perdre une image dans le tas
    {
        if(i == _imgStart)
            featDet.findKeypoints(_path + _name + _op.int2string(i) + _ext, 1);
        else
            featDet.switch2to1();

        featDet.findKeypoints(_path + _name + _op.int2string(i+1) + _ext, 2);
        featDet.findMatches();

        matches = featDet.getSortedMatches();
        kpt1 = featDet.getKeyPoints(1);
        kpt2 = featDet.getKeyPoints(2);

        findAffineTransform(matches, kpt1, kpt2);


    }

    for(int j = 0 ; j<_imgEnd ; j++)
    {
        createWarpedImages_it_mean(j, kpt1, kpt2, _op.meanMat(_affVector));
        createWarpedImages_it(j, kpt1, kpt2);
    }
}

void affineTransform::displayMatches(const std::vector<DMatch> &matches, const std::vector<KeyPoint> &keypoint_1, const std::vector<KeyPoint> &keypoint_2, const Mat &img1, const Mat &img2, const string path)
{
    Mat img_out;
    drawMatches(img1, keypoint_1, img2, keypoint_2, matches, img_out, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    imwrite(path, img_out);
}
