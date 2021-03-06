/*!
 * Author : Florian Lardeux - November 2015
 * videoManager is a simple, easily alterable class that allows the management of videos
 * and images.
 * For instance it can be used to create a video out of a sequence of images.
*/

#ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

#include "opencv2/opencv.hpp"
#include "mediamanager.h"

class videoManager : public MediaManager
{
public:

    //Constructors
    videoManager();
    videoManager(std::string path, std::string extension, int n_images, std::string name = "", std::string videoName = "");

    //Methods
    void setVideo();

    virtual void loadFile();
    virtual void saveFile();

    //Getters
    cv::VideoWriter getVideo();

    //Setters
    void setVideo(const cv::VideoWriter &video);
    //void setExt(const std::string &ext);
    //void setPath(const std::string &path);
    //void setName(const std::string &name);
    void setVideoName(const std::string &videoName);

private:
    cv::VideoWriter _video;
    int _nImages;
    //std::string _ext;
    //std::string _path;
    //std::string _name;
    std::string _videoName; //fileName;
};

#endif // VIDEOMANAGER_H
