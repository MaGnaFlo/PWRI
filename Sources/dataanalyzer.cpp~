#include "dataanalyzer.h"

using namespace std;

dataAnalyzer::dataAnalyzer() : _data(std::vector<double>())
{
}

dataAnalyzer::dataAnalyzer(const std::vector<double> &data)
{
    if(data.size() == 0)
        cout << "Size of data = 0 !" << endl;

    _data = data;
}

void dataAnalyzer::findMax()
{
    _maxPos.clear();

    //si le spectre commence en decroissant
    if(_data[0] >= _data[1] && _data[0] >= _data[2])
        _maxPos.push_back(0);

    for(unsigned int i = 2; i < _data.size()-2; i++)
        if(_data[i] >= _data[i+1] && _data[i] >= _data[i+2] && _data[i] >= _data[i-1] && _data[i] >= _data[i-2])
            _maxPos.push_back(i);

    //si le spectre finit en decroissant
    if(_data[_data.size()-1] >= _data[_data.size()-2] && _data[_data.size()-1] >= _data[_data.size()-3])
        _maxPos.push_back(_data.size()-1);
}

void dataAnalyzer::findRegions()
{
    int nbInter = _maxPos.size() - 1;
    _regions.clear();
    int j = 1;

    for(int i = 0; i < nbInter; i++)
    {
        if(_data[_maxPos[i]] < _data[_maxPos[i+1]])
        {
            j = _maxPos[i] + 1;
            while(_data[j] < _data[_maxPos[i]] && j<_maxPos[i+1])
                j++;
            reg region;
            region.x1 = _maxPos[i];
            region.x2 = j;

            _regions.push_back(region);
        }
        else
        {
            j = _maxPos[i+1] - 1;
            while(_data[j] < _data[_maxPos[i+1]] && j>=0)
                j--;
            reg region;
            region.x1 = j;
            region.x2 = _maxPos[i+1];
            _regions.push_back(region);
        }
    }
    return;
}

void dataAnalyzer::findBestMin(int &pos1, int &pos2)
{
    _bestRegions = std::vector<dataAnalyzer::reg>(0);
    std::vector<double> integ(_regions.size(), 0);
    for(unsigned int i = 0; i < _regions.size(); i++)
    {
        for(int j = _regions[i].x1; j <= _regions[i].x2; j++)
            integ[i] += _data[j];
        integ[i] -= (_data[_regions[i].x1]+_data[_regions[i].x2])*(_regions[i].x2 - _regions[i].x1 + 1)/2.;
    }

    int min1 = integ[0];
    unsigned int i1 = 0;
    for(unsigned int i = 0; i < integ.size(); i++)
        if(integ[i]<min1)
        {
            i1 = i;
            min1 = integ[i];
        }

    int min2 = (i1==0)?integ[1]:integ[0];
    unsigned int i2 = (i1==0)?1:0;
    for(unsigned int i = 0; i < integ.size(); i++)
        if(integ[i]<min2 && i != i1)
        {
            i2 = i;
            min2 = integ[i];
        }

    _bestRegions.push_back(_regions[i1]);
    _bestRegions.push_back(_regions[i2]);

    int k1 = _regions[i1].x1;
    while(_data[k1] > _data[k1+1] && k1 <= _regions[i1].x2)
        k1++;

    int k2 = _regions[i2].x1;
    while(_data[k2] > _data[k2+1] && k2 <= _regions[i2].x2)
        k2++;

    pos1 = (k1<k2)?k1:k2;
    pos2 = (pos1==k2)?k1:k2;

}

void dataAnalyzer::process(int &pos1, int &pos2)
{
    findMax();
    findRegions();
    findBestMin(pos1, pos2);
}

vector<int> dataAnalyzer::minDetection(int nbr_pic, vector<double> data)
{
    vector<int> minVector;
    vector<double>::iterator res;
    int res_int;
    vector<double>::iterator res_max = max_element(data.begin(), data.end());
    int max = distance(data.begin(), res_max);
    vector<double> t_data = data;
    int range = 50;

    if (nbr_pic == 1)
    {
        res = min_element(t_data.begin(), t_data.end());
        res_int = distance(t_data.begin(), res);
        minVector.push_back(res_int);
    }
    else
    {
        for (int i = 0;i<nbr_pic-1;i++)
        {
            res = min_element(t_data.begin(), t_data.end());
            res_int = distance(t_data.begin(), res);
            minVector.push_back(res_int);

            for(int j = res_int - range; j < res_int + range; j++)
            {
                if(j>0 && (unsigned int)j <t_data.size())
                    t_data[j] = t_data[max];
            }
        }
        res = min_element(t_data.begin(), t_data.end());
        res_int = distance(t_data.begin(), res);
        minVector.push_back(res_int);
    }
    _minVector = minVector;
    return minVector;
}

int dataAnalyzer::maxDetection(std::vector<double> data)
{
    std::vector<int>::iterator res = min_element(_minVector.begin(), _minVector.end());
    int res_int = distance(_minVector.begin(), res);
    int min_pos = _minVector[res_int];

    std::vector<double>::iterator res2 = max_element(data.begin(), data.begin() + min_pos);
    int res2_int = distance(data.begin(), res2);
    int maxPos = res2_int;

    return maxPos;
}

void dataAnalyzer::setData(const std::vector<double> &data)
{
    _data = data;
}
