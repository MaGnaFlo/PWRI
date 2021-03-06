/*!
 * \author : Florian Lardeux, Thomas Maifret, Victor Petitjean
 * November 2015
*/

#ifndef CALIBRATIONFILEMANAGER_H
#define CALIBRATIONFILEMANAGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstdio>
#include <stdio.h>
#include <cstdlib>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "mediamanager.h"


/*!
 * This class is a structure containing a setup of the experimentation.
 * It contains 4 arguments, and it's mainly used by the following class, ClibrationFileManager.
*/

class Setup
{
public:
    //Constructors
    Setup();
    Setup(int obj, int ldo, int ang, double ind);

    //Getters
    int getObj() {return _obj;}
    int getLdo() {return _ldo;}
    int getAng() {return _ang;}
    double getInd() {return _ind;}

    //Setters
    void setObj(int obj) { _obj = obj;}
    void setLdo(int ldo) { _ldo = ldo;}
    void setAng(int ang) { _ang = ang;}
    void setInd(double ind) { _ind = ind;}

private:
    int _obj; //Objective lens
    int _ldo; //Wavelength
    int _ang; //Top angle of the prism
    double _ind; //Index of the prism
};

/*!
 * This class is derived from MediaManager. Its purpose is to load and save calibration
 * files (ext = ".txt"), which contain data for a transformation matrix.
*/

class CalibrationFileManager : public FileManager
{
public:
    //Constructors
    CalibrationFileManager();
    CalibrationFileManager(int obj, int ldo, int ang, double ind);

    //Methods
    virtual void loadFile();
    virtual void saveFile();

    //Getters
    cv::Mat calibMat() const;
    Setup getSetup() {return _setup;}

    //Setters
    void setCalibMat(const cv::Mat &calibMat);
    void setIsStandard(const bool isStandard);

private:
    bool _isStandard;
    Setup _setup;
    cv::Mat _calibMat;
};

#endif // CALIBRATIONFILEMANAGER_H
