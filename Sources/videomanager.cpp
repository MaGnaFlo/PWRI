/*!
 * Author : Florian Lardeux - November 2015
 * videoManager is a simple, easily alterable class that allows the management of videos
 * and images.
 * For instance it can be used to create a video out of a sequence of images.
*/

#include "videomanager.h"

using namespace cv;
using namespace std;


///// CONSTRUCTORS /////

videoManager::videoManager()
{
    _path = "C:/";
    _ext = "";
    _nImages = 0;
    _name = "";
    _videoName = "";
}

    /*! videoManager loads a set of images via their path, their extension, the name of the images (if there is one)
     * and the name given to the video, if you want to create one.
     * Basically the name of an image must be : image_name 001.png for instance.
     * If the images are just "001.png", don't bother put a name, same thing for the video name.
     */

videoManager::videoManager(string path, string extension, int n_images, string name, string videoName)
{
    _path = path;
    _ext = extension;
    _nImages = n_images;
    _name = name;
    _videoName = videoName;
    setVideo();
}


///// METHODS /////

    /*! This method allows to create a video out of the image sequence you named in the constructor or
     * using the setters.
     */

void videoManager::setVideo()
{
    // firstImg is an image from which we extract the size
    Mat firstImg;
    firstImg = imread(_path + "/001." + _ext);
    _video.open(_path + "/" + _videoName + ".avi", -1, 15, Size(firstImg.size().width, firstImg.size().height), true);
}

/*! 
 * loadFile isn't supposed to be called in this class
*/

void videoManager::loadFile()
{

}

/*! 
 * saveFile goes through the set of images and saves the video.
 * /!\ setVideo has to be called before, to setup _video !
*/

void videoManager::saveFile()
{
    Mat img;
    string number;
    for(int i = 0 ; i<_nImages ; i++) {
        ostringstream convert;
        convert << i;
        if(i<10)
            number = "00" + convert.str();
        else if(i<100)
            number = "0" + convert.str();
        else
            number = convert.str();
        img = imread(_path + _name + number + _ext, CV_LOAD_IMAGE_GRAYSCALE);
        _video.write(img);
    }
}


///// GETTERS /////

cv::VideoWriter videoManager::getVideo()
{
    return _video;
}

/*void videoManager::setVideo(const cv::VideoWriter &video)
{
    _video = video;
}

void videoManager::setExt(const std::string &ext)
{
    _ext = ext;
}

void videoManager::setPath(const std::string &path)
{
    _path = path;
}

void videoManager::setName(const std::string &name)
{
    _name = name;
}*/


///// SETTERS /////

void videoManager::setVideoName(const std::string &videoName)
{
    _videoName = videoName;
}

