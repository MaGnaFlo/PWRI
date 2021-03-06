/*!
 * \authors : Florian Lardeux, Victor Petitjean, Thomas Maifret
 * November 2015
 * 
 * dataAnalyzer is used to analyze spectra, such as finding its minima.
*/

#ifndef DATAANALYZER_H
#define DATAANALYZER_H

#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <numeric>

class dataAnalyzer
{
public:
    //Struct
    struct reg {
        int x1;
        int x2;
    };

    //Constructors
    dataAnalyzer();
    dataAnalyzer(const std::vector<double> &data);

    //Methods
    void findMax();
    void findRegions();
    void findBestMin(int &pos1, int &pos2);
    void process(int &pos1, int &pos2);

    //Getters
    std::vector<double> getData() const {return _data;}
    std::vector<int> getMaxPos() const {return _maxPos;}

    //Setters
    void setData(const std::vector<double> &data);

    //Obsolete - kept to avoid conflicts
    std::vector<int> minDetection(int nbr_pic, std::vector<double> data);
    int maxDetection(std::vector<double> data);

private:
    std::vector<double> _data;
    std::vector<int> _maxPos;
    std::vector<reg> _regions;
    std::vector<reg> _bestRegions;

    //Obsolete - kept to avoid conflicts
    std::vector<int> _minVector;
};

#endif // DATAANALYZER_H
