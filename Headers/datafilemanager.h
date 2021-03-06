/*!
 * \authors : FlorianLardeux, Thomas Maifret, Victor Petitjean
 * November 2015
 *
 * DataFileManager derives from MediaManager. It can save text files containing
 * the raw data of a spectrum (x- and y-coordinates)
*/

#ifndef DATAFILEMANAGER_H
#define DATAFILEMANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "mediamanager.h"

class DataFileManager : public FileManager
{
public:
    //Constructors
    DataFileManager();
    DataFileManager(std::vector<double> dataX, std::vector<double> dataY, std::string path, std::string name);

    //Methods
    virtual void loadFile();
    virtual void saveFile();

    //Getters
    std::vector<double> dataX() const;
    std::vector<double> dataY() const;

    //Setters
    void setDataX(const std::vector<double> &dataX);
    void setDataY(const std::vector<double> &dataY);

private:
    std::vector<double> _dataX;
    std::vector<double> _dataY;
};

#endif // DATAFILEMANAGER_H
