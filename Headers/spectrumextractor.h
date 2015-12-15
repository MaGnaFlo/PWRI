#ifndef SPECTRUMEXTRACTOR_H
#define SPECTRUMEXTRACTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <QProgressBar>
#include <QApplication>

#include "operation.h"
#include "dataanalyzer.h"

#define PI 3.14159

class SpectrumExtractor
{
public:
    SpectrumExtractor();
    SpectrumExtractor(int imgStart, int imgEnd, double angleStart, double angleStep, unsigned int filterSize, double std, std::string path, std::string name, std::string ext);

    //Processes
    void processSpectrumAverage(QProgressBar *progressBar);
    void processSpectrumZone(int width, int height, int posX, int posY);
    void processSpectrumAllPixels(QProgressBar *progressBar);
    inline int borderCorrect(int x, int y);
    void noBorderProcessing(std::vector<double> filter, int posBlockX, int posBlockY);
    void indexing(std::vector<double> filter, int posBlockX, int posBlockY);
    void initAndProcess(int posBlockX, int posBlockY);
    inline void gaussianFilter(unsigned int &size, double &std, std::vector<double> &filter);

    //Getters
    std::vector<double> dataX() const;
    std::vector<double> dataY() const;
    std::vector<double> dataBlock() const;
    cv::Mat getMinP() const;
    cv::Mat getMinS() const;

    //Setter & getter for _dataBlock
    double getDataBlock(const int x, const int y, const int i);
    std::vector<double> getDataBlockSpectre(const int x, const int y);
    void setDataBlock(const int x, const int y, const int i, const double value);
    void setFilterSize(unsigned int filterSize);

    void setStd(double std);

private :
    std::vector<double> _dataX;
    std::vector<double> _dataY;
    std::vector<double> _dataBlock;
    cv::Mat _minP;
    cv::Mat _minS;
    std::string _pathS;
    std::string _ext;
    std::string _name;
    Operation _op;
    double _angleStart;
    double _angleStep;
    unsigned int _imgStart;
    unsigned int _imgEnd;
    unsigned int _width;
    unsigned int _height;
    unsigned int _sizeBlock;
    unsigned int _filterSize;
    double _std;
};

#endif // SPECTRUMEXTRACTOR_H
