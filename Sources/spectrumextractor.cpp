#include "spectrumextractor.h"

using namespace cv;
using namespace std;

SpectrumExtractor::SpectrumExtractor()
{

}

SpectrumExtractor::SpectrumExtractor(int imgStart, int imgEnd, double angleStart, double angleStep, unsigned int filterSize, double std, string path, string name, string ext)
{
    _imgStart = imgStart;
    _imgEnd = imgEnd;
    _angleStart = angleStart;
    _angleStep = angleStep;
    _pathS = path;
    _name = name;
    _ext = ext;
    _filterSize = filterSize;
    _std = std;

    int nbImages = _imgEnd - _imgStart;

    _dataX = vector<double>(nbImages,0.);
    _dataY = vector<double>(nbImages,0.);

    for(int i = 0; i<nbImages; i++)
        _dataX[i] = _angleStart + i*_angleStep;

    _minP = imread(_pathS + "Processed_"  + _name + _op.int2string(_imgStart) + _ext, IMREAD_GRAYSCALE);
    _width = _minP.size().width;
    _height = _minP.size().height;
    _minS = imread(_pathS + "Processed_"  + _name + _op.int2string(_imgStart) + _ext, IMREAD_GRAYSCALE);

    _sizeBlock = 120;
    _dataBlock = vector<double>(_sizeBlock*_sizeBlock*nbImages,0.);

    cout << "SpectrumExtractor::SpectrumExtractor : fin du constructeur" << endl;
}

double SpectrumExtractor::getDataBlock(const int x, const int y, const int i)
{
    assert(x>=0 && x<_sizeBlock);
    assert(y>=0 && y<_sizeBlock);

    int pos = i*_sizeBlock*_sizeBlock + y*_sizeBlock + x;
    assert(pos>=0 && pos<_dataBlock.size());

    return _dataBlock[pos];
}

std::vector<double> SpectrumExtractor::getDataBlockSpectre(const int x, const int y)
{
    assert(x>=0 && x<_sizeBlock);
    assert(y>=0 && y<_sizeBlock);

    for(unsigned int i = 0; i < _dataX.size(); i++)
        _dataY[i] = getDataBlock(x,y,i);

    return _dataY;
}

void SpectrumExtractor::setDataBlock(const int x, const int y, const int i, const double value)
{
    assert(x>=0 && x<_sizeBlock);
    assert(y>=0 && y<_sizeBlock);

    int pos = i*_sizeBlock*_sizeBlock + y*_sizeBlock + x;
    assert(pos>=0 && pos<_dataBlock.size());

    _dataBlock[pos] = value;
}

void SpectrumExtractor::setFilterSize(unsigned int filterSize)
{
    _filterSize = filterSize;
}

void SpectrumExtractor::setStd(double std)
{
    _std = std;
}

void SpectrumExtractor::processSpectrumAverage(QProgressBar *progressBar)
{
    int n_images = _dataX.size();
    Mat image;
    string is;

    if (_pathS.size() == 0)
    {
        cout << "SpectrumExtractor::processSpectrumAverage : Please select a \"save\" folder before using this function" << endl;
        return;
    }
    for (int i = _imgStart; i < _imgEnd; i++)
    {
        is = _op.int2string(i);
        // Read the file
        image = imread(_pathS + "Processed_"  + _name + is + _ext, IMREAD_GRAYSCALE);
        if (!image.data) // Check for invalid input
            cout << "SpectrumExtractor::processSpectrumAverage : Could not open or find the image" << std::endl;
        Scalar tscal = mean(image);
        float meanIm = tscal.val[0];
        _dataY[i-_imgStart] = meanIm;
        _dataX[i-_imgStart] = _angleStart + i*_angleStep;

        progressBar->setValue(i/(double)n_images*100);
        progressBar->repaint();
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

void SpectrumExtractor::processSpectrumZone(int width, int height, int posX, int posY)
{
    Mat image;
    string is;
    Mat zone = Mat::zeros(Size(width, height), CV_64F);

    int n_images = _dataX.size();

    for (int i = _imgStart; i < _imgEnd; i++)
    {
        is = _op.int2string(i);
        if(_pathS.size() == 0)
            cout << "MainWindow::setSpectre : Please select a \"save\" folder before using this function" << endl;
        // Read the file
        image = imread(_pathS + "Processed_"  + _name + is + _ext, IMREAD_COLOR);

        if (!image.data) // Check for invalid input
            cout << "MainWindow::setSpectre : Could not open or find the image" << std::endl;
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                zone.at<double>(x, y) = image.at<double>(posX + x, posY + y);
            }
        Scalar tscal = mean(zone);
        float meanIm = tscal.val[0];
        _dataY[i-_imgStart] = meanIm;
        _dataX[i-_imgStart] = _angleStart + i*_angleStep;
    }
}

void SpectrumExtractor::processSpectrumAllPixels(QProgressBar *progressBar)
{
    int nbImages = _dataX.size();
    dataAnalyzer da;
    if (_pathS.size() == 0)
    {
        cout << "SpectrumExtractor::processSpectrumAllPixels : Please select a \"save\" folder before using this function" << endl;
        return;
    }
    unsigned int nbBlockW = _width  / _sizeBlock;
    unsigned int nbBlockH = _height / _sizeBlock;
    unsigned int nProgressbar = _sizeBlock*_sizeBlock*nbBlockH*nbBlockW;

    for(unsigned int x = 0; x < nbBlockH; x++)
        for(unsigned int y = 0; y < nbBlockW; y++)
        {
            cout << "SpectrumExtractor::processSpectrumAllPixels : about to process block " << x << " " << y << endl;
            initAndProcess(x,y);
            for(unsigned int i = 0; i < _sizeBlock; i++)
                for(unsigned int j = 0; j < _sizeBlock; j++)
                {
                    int minP, minS;
                    da.setData(getDataBlockSpectre(i,j));
                    da.process(minP, minS);
                    _minP.at<uchar>(x*_sizeBlock+i, y*_sizeBlock+j) = (uchar)(minP*255./(double)nbImages);
                    _minS.at<uchar>(x*_sizeBlock+i, y*_sizeBlock+j) = (uchar)(minS*255./(double)nbImages);
                    progressBar->setValue(100*(j + i*_sizeBlock + y*_sizeBlock*_sizeBlock + x*_sizeBlock*_sizeBlock*nbBlockW)/(double)nProgressbar);
                    progressBar->repaint();
                    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
                }
        }
}

std::vector<double> SpectrumExtractor::dataX() const
{
    return _dataX;
}

std::vector<double> SpectrumExtractor::dataY() const
{
    return _dataY;
}

/*std::vector<std::vector<double> > SpectrumExtractor::dataZ() const
{
    return _dataZ;
}*/

std::vector<double> SpectrumExtractor::dataBlock() const
{
    return _dataBlock;
}

cv::Mat SpectrumExtractor::getMinP() const
{
    return _minP;
}

cv::Mat SpectrumExtractor::getMinS() const
{
    return _minS;
}

int SpectrumExtractor::borderCorrect(int x, int y)
{
    if(y < 0)
        return -y - 1;
    if(y >= x)
        return 2*x - y - 1;
    return y;
}

void SpectrumExtractor::noBorderProcessing(vector<double> filter, int posBlockX, int posBlockY)
{
    unsigned int size = sqrt(filter.size());
    float sum;
    Mat image;
    string is;

    for (int n = _imgStart; n < _imgEnd; n++)
    {
        is = _op.int2string(n);
        // Read the file
        image = imread(_pathS + "Processed_"  + _name + is + _ext, IMREAD_GRAYSCALE);
        if (!image.data) // Check for invalid input
            cout << "SpectrumExtractor::processSpectrumPixelBlock : Could not open or find the image" << std::endl;

        for (unsigned int x = _sizeBlock*posBlockX; x < _sizeBlock*(posBlockX+1); x++)
            for (unsigned int y = _sizeBlock*posBlockY ; y < _sizeBlock*(posBlockY+1); y++)
            {
                int xB = x - _sizeBlock*posBlockX;
                int yB = y - _sizeBlock*posBlockY;
                sum = 0.0;
                for(unsigned int i = 0; i < size;i++)
                    for(unsigned int j = 0; j < size; j++)
                    {
                        sum += filter[j + size*i]*image.at<uchar>(y + j - size/2, x + i - size/2);
                    }
                setDataBlock(xB, yB, n-_imgStart, sum);
            }
    }
}

void SpectrumExtractor::indexing(vector<double> filter, int posBlockX, int posBlockY)
{
    unsigned int size = sqrt(filter.size());
    float sum, x1, y1;
    int n_images = _dataX.size();
    Mat image;
    string is;

    for (int n = _imgStart; n < _imgEnd; n++)
    {
        is = _op.int2string(n);
        // Read the file
        image = imread(_pathS + "Processed_"  + _name + is + _ext, IMREAD_GRAYSCALE);
        if (!image.data) // Check for invalid input
            cout << "SpectrumExtractor::processSpectrumPixelBlock : Could not open or find the image" << std::endl;

        for (unsigned int x = _sizeBlock*posBlockX; x < _sizeBlock*(posBlockX+1); x++)
            for (unsigned int y = _sizeBlock*posBlockY ; y < _sizeBlock*(posBlockY+1); y++)
            {
                int xB = x - _sizeBlock*posBlockX;
                int yB = y - _sizeBlock*posBlockY;
                sum = 0.0;
                for(unsigned int i = 0; i < size; i++)
                    for(unsigned int j = 0; j < size; j++ )
                    {
                        x1 = borderCorrect(image.rows, x + i - size/2);
                        y1 = borderCorrect(image.cols, y + j - size/2);
                        sum = sum + filter[j + i*size]*image.at<uchar>(x1,y1);
                    }
                setDataBlock(xB, yB, n-_imgStart, sum);
            }
    }
}

void SpectrumExtractor::initAndProcess(int posBlockX, int posBlockY)
{
    vector<double> filter(_filterSize*_filterSize);
    gaussianFilter(_filterSize, _std, filter);
    indexing(filter, posBlockX, posBlockY);

    /*filter[0] = .025;
    filter[1] = .1;
    filter[2] = .025;
    filter[3] = .1;
    filter[4] = .5;
    filter[5] = .1;
    filter[6] = .025;
    filter[7] = .1;
    filter[8] = .025;*/
}

void SpectrumExtractor::gaussianFilter(unsigned int &size, double &std, std::vector<double> &filter)
{
    int x, y;
    for(unsigned int i = 0; i<size; i++)
        for(unsigned int j = 0; j<size; j++)
        {
            x = i - size/2;
            y = j - size/2;
            filter[j + i*size] = exp(-(x*x+y*y)/(2*std*std))/(2*PI*std*std);
        }
}
