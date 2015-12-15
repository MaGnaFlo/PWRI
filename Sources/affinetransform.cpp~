#include "affinetransform.h"

using namespace std;
using namespace cv;

affineTransform::affineTransform()
{
    _meanTransform = Mat::zeros(2,3,CV_64F);
    _affVector.push_back(Mat::zeros(2,3,CV_64F));
}

affineTransform::affineTransform(int imgStart, int imgEnd, string path, string name, string ext)
{
    _imgEnd = imgEnd;
    _imgStart = imgStart;
    _path = path;
    _name = name;
    _ext = ext;

    setMidXY();
    _meanTransform = Mat::zeros(2,3,CV_64F);
}

void affineTransform::setMidXY()
{
    Mat im = imread(_path + _name + _op.int2string(_imgStart) + _ext, IMREAD_GRAYSCALE);
    _midX = im.size().width / 2;
    _midY = im.size().height / 2;
    cout << "affineTransform::setMidXY : midX = " << _midX << " and midY = " << _midY << endl;
}

cv::Mat affineTransform::createWarpedImages_rec(const int &i)
{
    cout << "affineTransform::createWarpedImages_rec : Utilisation de createWarpedImages recursive" << endl;
    cout << "affineTransform::createWarpedImages_rec : i = " << i << endl;

    if(i == 1) {
        string number = _op.int2string(i + _imgStart - 1);
        Mat imgi = imread(_path + _name + number + _ext, IMREAD_GRAYSCALE);
        imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/" + number + _ext, imgi);
        return Mat::eye(3,3,CV_64F);
    }
    else {
        Mat transf3x3 = Mat::zeros(3,3,CV_64F);
        for(int m = 0 ; m<2 ; m++)
            for(int n = 0 ; n<3 ; n++)
                transf3x3.at<double>(m,n) = _affVector[i-2].at<double>(m,n);

        transf3x3.at<double>(2,2) = 1.f;

        Mat res = transf3x3 * createWarpedImages_rec(i - 1);

        Mat res2x3 = res.rowRange(0,2) / res.at<double>(2,2);

        string number = _op.int2string(i + _imgStart - 1);
        Mat imgi = imread(_path + _name + number + _ext, IMREAD_GRAYSCALE);
        Mat img_res;

        warpAffine(imgi, img_res, res2x3, imgi.size());
        imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/" + number + _ext, img_res);
        return img_res;
    }
}

void affineTransform::createWarpedImages_it()
{
    cout << "affineTransform::createWarpedImages_it : Utilisation de createWarpedImages iterative (sans moyenne)" << endl;

    Mat trans_product;
    Mat t_3x3;
    Mat trans_product_2x3;
    string number;
    Mat imgi;
    Mat img_warped;


    for(int j = _imgStart ; j<_imgEnd ; j++)
    {
        trans_product = Mat::eye(3,3,CV_64F);
        for(int i = 0 ; i<j-_imgStart ; i++) {
            t_3x3 = _op.convert2x3to3x3(_affVector[i]);
            trans_product = trans_product * t_3x3;
        }
        trans_product_2x3 = trans_product.rowRange(0,2) / trans_product.at<double>(2,2);
        number = _op.int2string(j);
        imgi = imread(_path + _name + number + _ext, IMREAD_GRAYSCALE);

        warpAffine(imgi, img_warped, trans_product_2x3, imgi.size());
        imwrite("C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/" + number + ".png", img_warped); 
    }
}

void affineTransform::createWarpedImages_it(const string &pathSave, QProgressBar *progressBar)
{
    cout << "affineTransform::createWarpedImages_it : Utilisation de createWarpedImages iterative (avec moyenne)" << endl;
    Mat trans_product;
    Mat t_3x3 = _op.convert2x3to3x3(_meanTransform);
    Mat trans_product_2x3;
    string number;
    Mat imgi;
    Mat img_warped;

    for(int j = _imgStart ; j<_imgEnd ; j++)
    {
        trans_product = Mat::eye(3,3,CV_64F);
        for(int i = 0 ; i<j-_imgStart ; i++) {
            trans_product = trans_product * t_3x3;
        }
        trans_product_2x3 = trans_product.rowRange(0,2) / trans_product.at<double>(2,2);

        number = _op.int2string(j);
        imgi = imread(_path + _name + number + _ext, IMREAD_GRAYSCALE);

        warpAffine(imgi, img_warped, trans_product_2x3, imgi.size());
        imwrite(pathSave + "Processed_" + _name + number + ".png", img_warped);
        progressBar->setValue((j-_imgStart)/(double)(_imgEnd-_imgStart)*100);
        progressBar->repaint();
    }
}

void affineTransform::findAffineTransform(const std::vector<DMatch> &matches, const std::vector<KeyPoint> &keypoint_1, const std::vector<KeyPoint> &keypoint_2)
{
    vector<DMatch> good_matches;
    Point2f coord1[3], coord2[3];
    unsigned int j = 0;
    int seuil = min(_midX, _midY)/2;

    while(true) {
        good_matches.clear();
        j = 0;

        while(keypoint_1[matches[j].queryIdx].pt.x > _midX-seuil
              || keypoint_1[matches[j].queryIdx].pt.y > _midY-seuil)
        {
              j++;
              if(j>=matches.size())
              {
                  cout << "affineTransform::findAffineTransform : 1st match is found too far" << endl;
                  break;
              }
        }
        if(j<matches.size())
        {
            good_matches.push_back(matches[j]);
            cout << "affineTransform::findAffineTransform : 1st match ok" << endl;
        }

        j = 0;
        while(keypoint_1[matches[j].queryIdx].pt.x < _midX+seuil
              || keypoint_1[matches[j].queryIdx].pt.y > _midY-seuil)
        {
              j++;
              if(j>=matches.size())
              {
                  cout << "affineTransform::findAffineTransform : 2nd match is found too far" << endl;
                  break;
              }
        }
        if(j<matches.size())
        {
            good_matches.push_back(matches[j]);
            cout << "affineTransform::findAffineTransform : 2nd match ok" << endl;
        }

        j = 0;
        while(keypoint_1[matches[j].queryIdx].pt.y < _midY+seuil)
        {
              j++;
              if(j>=matches.size())
              {
                  cout << "affineTransform::findAffineTransform : 3rd match is found too far" << endl;
                  break;
              }
        }
        if(j<matches.size())
        {
            good_matches.push_back(matches[j]);
            cout << "affineTransform::findAffineTransform : 3rd match ok" << endl;
        }
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
    Mat affineTrans = getAffineTransform(coord2, coord1);
    _affVector.push_back(affineTrans);
    std::cout << "affineTransform::findAffineTransform : " << affineTrans << endl;
}

void affineTransform::process(QProgressBar *progressBar)
{
    featureDetector featDet;
    vector<DMatch> matches;
    vector<KeyPoint> kpt1, kpt2;

    _affVector.clear();

    cout << "affineTransform::process : _imgStart = " << _imgStart << " and _imgEnd = " << _imgEnd << endl;
    for(int i = _imgStart ; i<_imgEnd-1 ; i++)
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
        displayFeatures(kpt1, kpt2, i);

        progressBar->setValue(i);
        progressBar->repaint();
    }
    _meanTransform = mean(_affVector);
}

void affineTransform::displayMatches(const std::vector<DMatch> &matches, const std::vector<KeyPoint> &keypoint_1, const std::vector<KeyPoint> &keypoint_2, const Mat &img1, const Mat &img2, const string path)
{
    Mat img_out;
    drawMatches(img1, keypoint_1, img2, keypoint_2, matches, img_out, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    imwrite(path, img_out);
}

void affineTransform::displayFeatures(const std::vector<KeyPoint> &kpt1, const std::vector<KeyPoint> &kpt2, int i) {

    Mat img1 = imread(_path + _name + _op.int2string(i) + _ext, IMREAD_GRAYSCALE);
    Mat img2 = imread(_path + _name + _op.int2string(i+1) + _ext, IMREAD_GRAYSCALE);
    cout << _path + _name + _op.int2string(i) + _ext << endl;
    displayMatches(_goodMatches, kpt1, kpt2, img1, img2, "C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/res/features/" + _op.int2string(i) + ".png");
}

void affineTransform::setMeanTransform(const cv::Mat &meanTransform)
{
    _meanTransform = meanTransform;
}

void affineTransform::setImgStart(const int &imgStart)
{
    _imgStart = imgStart;
}

void affineTransform::setImgEnd(const int &imgEnd)
{
    _imgEnd = imgEnd;
}

void affineTransform::setPath(const std::string &path)
{
    _path = path;
}

void affineTransform::setName(const std::string &name)
{
    _name = name;
}

void affineTransform::setExt(const std::string &ext)
{
    _ext = ext;
}

cv::Mat affineTransform::meanTransform() const
{
    return _meanTransform;
}

Mat affineTransform::mean(std::vector<Mat> vect) const
{
    Mat res = Mat::zeros(vect.at(0).size(), vect.at(0).type());
    for(unsigned int i = 0; i<vect.size(); i++)
        res += vect.at(i)/(double)vect.size();
    return res;
}
