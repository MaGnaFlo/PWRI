/*!
 * \file videomanager.cpp
 * \brief Classe pour gérer les I/O de vidéos.
 * \author {F. Lardeux, T. Maifret, V. Petitjean}
 * \date 20/11/2015
 *
 * Classe pour gérer les I/O de vidéos (dont transformer un set d'images en vidéo)
 */

#include "videomanager.h"

using namespace cv;
using namespace std;

/*!
 * \fn videoManager::videoManager()
 * \brief Constructeur 1 par défaut
 *
 * Constructeur sans rien (pas utile en l'état)
 */

videoManager::videoManager()
{
    _path = "C:/";
    _ext = "";
    _nImages = 0;
    _name = "";
    _videoName = "";
}

/*!
 * \fn videoManager::videoManager(string path, string extension, int n_images, string name, string videoName)
 * \brief Constructeur 2 par passage d'arguments
 *
 * \param path : chemin d'accès au dossier (se termine par '\')
 * \param extension : extension (commence par '.')
 * \param n_images : nombres d'images dans le set
 * \param name : nom des images avant le num (ex : si "Image ", alors l'image 12 est nommée "Image 012")
 * \param videoName : nom de la video créée
 *
 * Constructeur utile de la classe
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

/*!
 * \fn videoManager::videoManager(VideoCapture capt)
 * \brief Constructeur 3 INUTILISE
 *
 * \param capt : instance de la classe VideoCapture
 *
 * INUTILISE
 */

videoManager::videoManager(VideoCapture capt)
{

}

/*!
 * \fn void videoManager::createVideo()
 * \brief écrit la vidéo avec les paramètres qu'il a en attributs
 *
 * prend les n images du dossier, et les transforme en vidéo (nuances de gris)
 */

void videoManager::createVideo()
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
        img = imread(_path + "/" + _name + number + "." + _ext, CV_LOAD_IMAGE_GRAYSCALE);

        _video.write(img);

    }
}

/*!
 * \fn void videoManager::setVideo()
 * \brief instancie l'objet I/O de vidéo
 *
 * on renseigne ici les fps de la vidéo créée
 */

void videoManager::setVideo()
{
    // firstImg is an image from which we extract the size
    Mat firstImg;
    firstImg = imread(_path + "/000." + _ext);
    _video.open(_path + "/" + _videoName + ".avi", -1, 5, Size(firstImg.size().width, firstImg.size().height), true);
}

/*!
 * \fn cv::VideoWriter videoManager::getVideo()
 * \brief renvoie l'instance _video
 *
 * \return l'instance _video de I/O de video
 */

cv::VideoWriter videoManager::getVideo()
{
    return _video;
}

/*!
 * \brief plusieurs fonctions de set
 */

void videoManager::setVideo(const cv::VideoWriter &video)
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
}

void videoManager::setVideoName(const std::string &videoName)
{
    _videoName = videoName;
}

