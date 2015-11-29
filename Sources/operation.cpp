/*!
 * \file operation.cpp
 * \brief Classe d'opérations générales.
 * \author {F. Lardeux, T. Maifret, V. Petitjean}
 * \date 20/11/2015
 *
 * Classe comprenant plusieurs opérations basiques, telles que des modifications de matrices, de chaînes de caractres ...
 */


#include "operation.h"
#include <iostream>
#include <istream>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

/*!
 * \fn Operation::Operation()
 * \brief Constructeur
 */

Operation::Operation()
{
}

/*!
 * \fn std::string Operation::int2string(int n)
 * \brief Int vers String
 *
 * \param n : entier à transformer en string
 * \return chaine de caractères
 *
 * Renvoie une chaine de 3 caractères correspondant à l'entier n (ex : 12 -> "012")
 */

std::string Operation::int2string(int n)
{
    std::string number;
    std::ostringstream convert;
    convert << n;
    if(n<10)
        number = "00" + convert.str();
    else if(n<100)
        number = "0" + convert.str();
    else
        number = convert.str();

    return number;
}

/*!
 * \fn cv::Mat Operation::convert2x3to3x3(cv::Mat transf2x3)
 * \brief Matrice 2x3 en matrice 3x3
 *
 * \param transf2x3 : matice de transformation 2x3
 * \return matrice de transformation homogène 3x3
 *
 * Ajout d'une ligne [0 0 1] pour rendre la matrice de transfo 2x3 homogène
 */

cv::Mat Operation::convert2x3to3x3(cv::Mat transf2x3)
{
    cv::Mat transf3x3 = cv::Mat::zeros(3,3,CV_64F);
    for(int m = 0 ; m<2 ; m++)
        for(int n = 0 ; n<3 ; n++)
            transf3x3.at<double>(m,n) = transf2x3.at<double>(m,n);

    transf3x3.at<double>(2,2) = 1.f;

    return transf3x3;
}

/*!
 * \fn cv::Mat Operation::meanMat(const std::vector<cv::Mat> matrices)
 * \brief Moyenne d'un vecteur de Matrices
 *
 * \param matrices : vecteur de Matrices nxm
 * \return une matrice mxn, moyenne des matrices du vecteur paramètre
 */

cv::Mat Operation::meanMat(const std::vector<cv::Mat> matrices)
{
    assert(matrices.size() != 0);
    cv::Mat mean = matrices.at(0) / (double) matrices.size();
    for(unsigned int i = 1 ; i<matrices.size() ; i++)
        mean += matrices.at(i) / (double) matrices.size();
    return mean;
}

void Operation::readFileName(std::string filePath, std::string &path, std::string &name, int &imgStart, std::string &ext)
{
    int i = filePath.length() - 1;
        string c = "a";
        //Recherche de l'extension
        while (c != ".")
        {
            c = filePath.substr(i, 1);
            i--;
        }
        int iExt = i+1;
        ext = filePath.substr(iExt, filePath.length() - iExt + 1);
        //Recherche de ImgStart
        i -= 3;
        int iImgStart = i+1;
        imgStart = string2int(filePath.substr(iImgStart, 3));
        //Recherche du nom
        while (c != "/")
        {
            c = filePath.substr(i, 1);
            i--;
        }
        int iName = i+2;
        name = filePath.substr(iName, iImgStart - iName);
        //Recherche du chemin
        path = filePath.substr(0, iName);
}

int Operation::string2int(const string imgStart)
{
    if(imgStart.length() != 3)
        return 0;

    int c = 0, d = 0, u = 0;
    int constASCII = 48;

    //Chiffre des centaines
    char ch = imgStart.substr(0,1).c_str()[0];
    c = ch - constASCII;
    if(c < 0 || c > 9)
        return 0;

    ch = imgStart.substr(1,1).c_str()[0];
    d = ch - constASCII;
    if(d < 0 || d > 9)
        return 0;

    ch = imgStart.substr(2,1).c_str()[0];
    u = ch - constASCII;
    if(u < 0 || u > 9)
        return 0;

    return 100*c + 10*d + u;
}

int Operation::findlastImage(string path, string name, string ext, int imgStart)
{
    string mypath = path + name + int2string(imgStart) + ext;
    struct stat buffer;
    bool b = stat(mypath.c_str(), &buffer);
    int i = imgStart;

    while(!b)
    {
        ++i;
        mypath = path + name + int2string(i) + ext;
        b = stat(mypath.c_str(), &buffer);
    }
    return i-1;
}
