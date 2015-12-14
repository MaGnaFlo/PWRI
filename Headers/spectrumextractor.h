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

#include "operation.h"
#include "dataanalyzer.h"

class SpectrumExtractor
{
public:
    SpectrumExtractor();
    SpectrumExtractor(int imgStart, int imgEnd, double angleStart, double angleStep, std::string path, std::string name, std::string ext);

    //Setter & getter for _dataBlock
    double getDataBlock(const int x, const int y, const int i);
    std::vector<double> getDataBlockSpectre(const int x, const int y);
    void setDataBlock(const int x, const int y, const int i, const double value);

    //Processes
    void processSpectrumAverage(QProgressBar *progressBar);
    void processSpectrumZone(int width, int height, int posX, int posY);
    void processSpectrumPixelBlock(int posBlockX, int posBlockY);
    void processSpectrumAllPixels(QProgressBar *progressBar);
    int borderCorrect(int x, int y);
    void noBorderProcessing(std::vector<double> filter, int posBlockX, int posBlockY);
    void indexing(std::vector<double> filter, int posBlockX, int posBlockY);
    void initAndProcess(int posBlockX, int posBlockY);


    //Getters
    std::vector<double> dataX() const;
    std::vector<double> dataY() const;
    //std::vector<std::vector<double> > dataZ() const;
    std::vector<double> dataBlock() const;

    cv::Mat getMinP() const;
    cv::Mat getMinS() const;


private :
    std::vector<double> _dataX;
    std::vector<double> _dataY;
    //std::vector< std::vector<double> > _dataZ;
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
};

#endif // SPECTRUMEXTRACTOR_H
