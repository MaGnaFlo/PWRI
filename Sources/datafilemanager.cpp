/*!
 * \authors : FlorianLardeux, Thomas Maifret, Victor Petitjean
 * November 2015
 *
 * DataFileManager derives from MediaManager. It can save text files containing
 * the raw data of a spectrum (x- and y-coordinates)
*/

#include "datafilemanager.h"

using namespace std;


///// CONSTRUCTORS /////

DataFileManager::DataFileManager()
{

}

DataFileManager::DataFileManager(std::vector<double> dataX, std::vector<double> dataY, std::string path, std::string name)
{
    cout << _dataX.empty() << endl;
    _dataX = dataX;
    _dataY = dataY;
    _path = path;
    _name = name;
}


///// METHODS /////

/*!
 * loadFile isn't supposed to be called here !
*/

void DataFileManager::loadFile()
{
    cout << "DataFileManager::loadFile : on n'est pas cense etre la !!!" << endl;
}

/*!
 * saveFile saves a text file containing two columns separated by a tab :
 *  - the 1st column contains the x-coord (aka the angle)
 *  - the 2nd column contains the y-coord (aka the intensity of the spectrum)
 * A header containing an ever so brief summary of each column is inserted.
*/

void DataFileManager::saveFile()
{
    if(_dataX.empty())
        return;

    fstream fs;
    fs.open(fullPath().c_str(), ios::out | ios::trunc);
    unsigned int length = _dataX.size();

    if(!fs.is_open() || length == 0)
    {
        _isOpen = false;
        return;
    }
    else
    {
        fs << "Angle\tIntensity\n";
        for(unsigned int i = 0; i<length ; i++)
        {
            fs << _dataX[i] << "\t" << _dataY[i] << "\n";
        }
        fs.close();
        _isOpen = true;
    }
}


///// GETTERS /////

std::vector<double> DataFileManager::dataX() const
{
    return _dataX;
}

std::vector<double> DataFileManager::dataY() const
{
    return _dataY;
}


///// SETTERS /////

void DataFileManager::setDataX(const std::vector<double> &dataX)
{
    _dataX = dataX;
}

void DataFileManager::setDataY(const std::vector<double> &dataY)
{
    _dataY = dataY;
}
