/*
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
    virtual void loadFile();
    virtual void saveFile();

    /* Load a set of images via their path, their extension, the name of the images (if there is one)
     * and the name given to the video, if you want to create one.
     * Basically the name of an image must be : image_name 001.png for instance.
     * If the images are just "001.png", don't bother put a name, same thing for the video name.
     */
    videoManager(std::string path, std::string extension, int n_images, std::string name = "", std::string videoName = "");

    /* This method allows to create a video out of the image sequence you named in the constructor or
     * using the setters.
     */
    void setVideo();
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
