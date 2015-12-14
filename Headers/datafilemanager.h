#ifndef DATAFILEMANAGER_H
#define DATAFILEMANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "mediamanager.h"

class DataFileManager : public FileManager
{
public:
    DataFileManager();
    DataFileManager(std::vector<double> dataX, std::vector<double> dataY, std::string path, std::string name);
    virtual void loadFile();
    virtual void saveFile();

    std::vector<double> dataX() const;
    std::vector<double> dataY() const;
    void setDataX(const std::vector<double> &dataX);
    void setDataY(const std::vector<double> &dataY);

private:
    std::vector<double> _dataX;
    std::vector<double> _dataY;
};

#endif // DATAFILEMANAGER_H
