/*!
 * Authors : Florian Lardeux, Thomas Maifret, Victor Petitjean
 * November 2015
 *
 * Operation is a carryall class that contains useful but simple and
 * general operations.
 * These operations can for instance convert int to/from string,
 * calculate the mean of a set of matrices ...
*/

#ifndef OPERATION_H
#define OPERATION_H

#include <stdio.h>
#include <iostream>
#include <istream>
#include <sys/stat.h>
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>


class Operation
{
public:
    //Constructors
    Operation();
	
	/*!
	 * int2string simply converts an int between 0 & 999 into a string of 3 characters
	 * (like "012")
	*/
    std::string int2string(int n);
	
	/*!
	 * convert2x3to3x3 converts an affine transformation matrix (2x3) into a homogeneous
	 * matrix (3x3). The last line of the resulting matrix is {0,0,1}
	*/
    cv::Mat convert2x3to3x3(cv::Mat transf2x3);
	
	/*!
	 * meanMat returns the mean of a vector of matrices (with the same size)
	*/
    cv::Mat meanMat(const std::vector<cv::Mat> matrices);
	
	/*!
	 * readFileName returns (in the references) the path, filename, number and extension of a file whose title
	 * looks like "C:/folder1/folder2/name of the file 012.png" for example. In such a case, 
	 *    path     = "C:/folder1/folder2/"
	 *    name     = "name of the file "
	 *    imgStart = 12
	 *  & ext      = ".png"
	 *
	 * If the file doesn't contain a number, withNumber can be set to false and then, no number will be expected
	*/
    void readFileName(std::string filePath, std::string &path, std::string &name, int &imgStart, std::string& ext, bool withNumber);
	
	/*!
	 * string2int return the int corresponding to the 3-character string ("012" gives 12)
	*/
    int string2int(const std::string imgStart);
	
	/*!
	 * findlastImage looks into the folder of the file [path+name+imgStart+ext], and searches for
	 * the last image with the same name (but with a different number)
	*/
    int findlastImage(std::string path, std::string name, std::string ext, int imgStart);
	
	/*!
	 * minAndMaxVectorDouble takes a vector of double, and returns (in the references)
	 * the min and max of the vector (the values, not the positions)
	*/
    void minAndMaxVectorDouble(const std::vector<double> &vec, double &m, double &M);
};

#endif // OPERATION_H
