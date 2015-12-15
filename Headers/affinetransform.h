/*!
 * Authors : Florian Lardeux, Thomas Maifret, Victor Petitjean
 * November 2015
 *
 * affineTransform is a complete class that calculates the affine transformation between images
 * using feature detection & feature matching. It also saves the undistorted images.
 * It relies on the FeatureDetector class to detect features with OpenCV tools
*/

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
	
	/*!
	 * This constructor sets all the images that ar going to be processed.
	 * The images go from [path + name + imgStart + ext] to [path + name + imgEnd + ext]
	*/
    affineTransform(int imgStart, int imgEnd, std::string path, std::string name, std::string ext);

    //Methods
	
	/*!
	 * setMidXY sets the middle of the images (height and width over 2)
	*/
    void setMidXY();
	
	/*!
	 * UNUSED createWarpedImages_rec
	 * UNUSED createWarpedImages_it()
	 * createWarpedImages_it(const std::string &pathSave, QProgressBar *progressBar)
	 * These 3 methods save the processed (aka undistorded) images, either the process being recursive or iterative
	 * createWarpedImages_it takes a saving path. The processed images have the name [pathSave + "Processed_" + name + number + ext]
	 * The methods also takes a pointer to a QProgressBar, to update it while processing
	*/
    cv::Mat createWarpedImages_rec(const int &i);
    void createWarpedImages_it();
    void createWarpedImages_it(const std::string &pathSave, QProgressBar *progressBar);
	
	/*!
	 * findAffineTransform calculates the transform matrix between two (successive) images
	 * Feature matches (pairs of keypoints) have been previously calculated, and the methods selects the best pairs
	 * to calculate the best transformation matrix
	*/
    void findAffineTransform(const std::vector<cv::DMatch> &matches, const std::vector<cv::KeyPoint> &keypoint_1, const std::vector<cv::KeyPoint> &keypoint_2);
    
	/*!
	 * process is the method to call to calculate the mean transformation matrix of an entire set.
	 * It calls findAffineTransform for each pair of images, after having found the keypoints and matches.
	 * Then it returns the mean transformation matrix
	*/
	void process(QProgressBar *progressBar);
	
	/*!
	 * displayMatches
	 * displayFeatures
	 * These 2 methods display the keypoints and/or matches, to verify the results returned by the OpenCV tools
	*/
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
