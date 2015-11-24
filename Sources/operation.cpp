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
    for(int i = 1 ; i<matrices.size() ; i++)
        mean += matrices.at(i) / (double) matrices.size();

    return mean;
}

