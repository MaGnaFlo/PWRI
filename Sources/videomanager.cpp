#include "videomanager.h"

using namespace cv;
using namespace std;

videoManager::videoManager(string path, string extension, int n_images, string name, string videoName)
{
    _path = path;
    _ext = extension;
    _nImages = n_images;
    _name = name;
    _videoName = videoName;
    setVideo();
}

videoManager::videoManager()
{
    _path = "C:/";
    _ext = "";
    _nImages = 0;
    _name = "";
    _videoName = "";
}

void videoManager::loadFile()
{

}

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

void videoManager::setVideo()
{
    // firstImg is an image from which we extract the size
    Mat firstImg;
    firstImg = imread(_path + "/001." + _ext);
    _video.open(_path + "/" + _videoName + ".avi", -1, 15, Size(firstImg.size().width, firstImg.size().height), true);
}

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

void videoManager::setVideoName(const std::string &videoName)
{
    _videoName = videoName;
}

