#include "datafilemanager.h"

using namespace std;

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

void DataFileManager::loadFile()
{
    cout << "DataFileManager::loadFile : on n'est pas cense etre la !!!" << endl;
}

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

std::vector<double> DataFileManager::dataX() const
{
    return _dataX;
}

void DataFileManager::setDataX(const std::vector<double> &dataX)
{
    _dataX = dataX;
}

void DataFileManager::setDataY(const std::vector<double> &dataY)
{
    _dataY = dataY;
}

std::vector<double> DataFileManager::dataY() const
{
    return _dataY;
}


