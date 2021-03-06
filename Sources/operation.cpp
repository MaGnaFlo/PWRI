/*!
 * Authors : Florian Lardeux, Thomas Maifret, Victor Petitjean
 * November 2015
 *
 * Operation is a carryall class that contains useful but simple and
 * general operations.
 * These operations can for instance convert int to/from string,
 * calculate the mean of a set of matrices ...
*/

#include "operation.h"

using namespace std;


///// CONSTRUCTOR /////
Operation::Operation()
{
}


///// METHODS /////

/*!
	 * int2string simply converts an int between 0 & 999 into a string of 3 characters
	 * (like "012")
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
	 * convert2x3to3x3 converts an affine transformation matrix (2x3) into a homogeneous
	 * matrix (3x3). The last line of the resulting matrix is {0,0,1}
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
	 * meanMat returns the mean of a vector of matrices (with the same size)
	*/

cv::Mat Operation::meanMat(const std::vector<cv::Mat> matrices)
{
    assert(matrices.size() != 0);
    cv::Mat mean = matrices.at(0) / (double) matrices.size();
    for(unsigned int i = 1 ; i<matrices.size() ; i++)
        mean += matrices.at(i) / (double) matrices.size();
    return mean;
}

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

void Operation::readFileName(std::string filePath, std::string &path, std::string &name, int &imgStart, std::string &ext, bool withNumber)
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

        int iImgStart;
        if(withNumber) //Recherche de ImgStart
        {
            i -= 3;
            iImgStart = i+1;
            imgStart = string2int(filePath.substr(iImgStart, 3));
        }
        else
            iImgStart = i+1;

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

/*!
	 * string2int return the int corresponding to the 3-character string ("012" gives 12)
	*/

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

    //Chiffre des dizaines
    ch = imgStart.substr(1,1).c_str()[0];
    d = ch - constASCII;
    if(d < 0 || d > 9)
        return 0;

    //Chiffre des unités
    ch = imgStart.substr(2,1).c_str()[0];
    u = ch - constASCII;
    if(u < 0 || u > 9)
        return 0;

    return 100*c + 10*d + u;
}

/*!
	 * findlastImage looks into the folder of the file [path+name+imgStart+ext], and searches for
	 * the last image with the same name (but with a different number)
	*/

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

/*!
	 * minAndMaxVectorDouble takes a vector of double, and returns (in the references)
	 * the min and max of the vector (the values, not the positions)
	*/

void Operation::minAndMaxVectorDouble(const std::vector<double> &vec, double &m, double &M)
{
    if(vec.size() == 0)
    {
        cout << "Operation::minAndMaxVectorDouble : Oups ! Empty vector !" << endl;
        m = 0;
        M = 0;
    }
    m = vec[0];
    M = vec[0];

    for(unsigned int i = 0; i<vec.size(); i++)
    {
        if(vec[i] < m)
            m = vec[i];
        if(vec[i] > M)
            M = vec[i];
    }
    cout << "Operation::minAndMaxVectorDouble : Finally, m = " << m << " and M = " << M << endl;
}
