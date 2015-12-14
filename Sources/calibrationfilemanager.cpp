#include "calibrationfilemanager.h"

using namespace std;

CalibrationFileManager::CalibrationFileManager()
{
    _calibMat = cv::Mat::zeros(2,3, CV_64F);
    _path = "CalibrationFiles/";
}

CalibrationFileManager::CalibrationFileManager(int obj, int ldo, int ang, double ind)
{
    _setup = Setup(obj, ldo, ang, ind);
    _path = "debug/CalibrationFiles/";
    _calibMat = cv::Mat::zeros(2,3, CV_64F);
}

void CalibrationFileManager::loadFile()
{
    fstream fs;
    string line;
    string fileName;
    string ext = ".txt";
    //This condition assess if the user wants to use the standard calibration file system or a specific one he chose.
    //
    if(_isStandard)
    {
        stringstream ss_name;
        ss_name << "Calib_";
        ss_name << _setup.getObj() << "x_" ;
        ss_name << _setup.getLdo() << "nm_" ;
        ss_name << _setup.getAng() << "deg_" ;
        ss_name << _setup.getInd() ;

        fileName = _path + ss_name.str() + ext;
    }
    else
        fileName = _path + _name + ext;

    cout << "CalibrationFileManager::readCalibrationFile : File about to be open : " << fileName << endl;

    fs.open(fileName.c_str(), fstream::in);
    if(!fs.is_open())
    {
        cout << "CalibrationFileManager::readCalibrationFile : Non-existing file" << endl;
        _isOpen = false;
        return;
    }
    getline(fs, line);

    if(line.c_str()[0] != '#')
    {
        cout << "CalibrationFileManager::readCalibrationFile : Oups ! The calibration file seems to be ill-written !" << endl;
        _isOpen = false;
        return;
    }
    getline(fs, line); //Empty line

    for(int y = 0; y < 2; y++)
        for(int x = 0; x < 3; x++)
        {
            getline(fs, line);
            _calibMat.at<double>(y,x) = atof(line.c_str());
        }
    _isOpen = true;
}

void CalibrationFileManager::saveFile()
{
    fstream fs;
    string ext = ".txt";
    string fileName;
    string fileNameSauvegarde;

    if(_isStandard)
    {
        stringstream ss_name;
        ss_name << "Calib_";
        ss_name << _setup.getObj() << "x_" ;
        ss_name << _setup.getLdo() << "nm_" ;
        ss_name << _setup.getAng() << "deg_" ;
        ss_name << _setup.getInd() ;
        fileName = _path + ss_name.str() + ext;
        fileNameSauvegarde = _path + ss_name.str() + "_SAUVEGARDE" + ext;
    }
    else
    {
        fileName = _path + _name + ext;
        fileNameSauvegarde = _path + _name + "_SAUVEGARDE" + ext;
    }

    cout << "CalibrationFileManager::writeCalibrationFile : file to be open = " << fileName << endl;
    struct stat buffer;
    bool exists = stat(fileName.c_str(), &buffer) == 0;

    if(exists)
    {
        cout << "CalibrationFileManager::writeCalibrationFile : File already there. Creating a saving file." << endl;

        string oldname = fileName;
        string newname = fileNameSauvegarde;
        bool pb = rename(oldname.c_str(), newname.c_str());

        if(!pb)
            cout << "CalibrationFileManager::writeCalibrationFile : Previous file saved under the name " << endl << newname << endl;
        else
            cout << "CalibrationFileManager::writeCalibrationFile : Oups ! There was a problem while saving the previous file." << endl;
    }
    fs.open(fileName.c_str(), fstream::out | fstream::trunc);

    if(fs.is_open())
    {
        fs << "#Calibration /!\\NE PAS MODIFIER/!\\ /!\\DO NOT MODIFY/!\\" << "\n";
        fs << "\n" ;
        for(int y = 0; y < 2; y++)
            for(int x = 0; x < 3; x++)
                fs << _calibMat.at<double>(y,x) << "\n" ;

        cout << "CalibrationFileManager::writeCalibrationFile : File " << fileName << " created." << endl;
        fs.close();
        _isOpen = true;
        return;
    }
    fs.close();
    _isOpen = false;
}

cv::Mat CalibrationFileManager::calibMat() const
{
    return _calibMat;
}

void CalibrationFileManager::setCalibMat(const cv::Mat &calibMat)
{
    _calibMat = calibMat;
}

void CalibrationFileManager::setIsStandard(bool isStandard)
{
    _isStandard = isStandard;
}

Setup::Setup() :
    _obj(10), _ldo(600), _ang(50), _ind(1.5)
{
}

Setup::Setup(int obj, int ldo, int ang, double ind) :
    _obj(obj), _ldo(ldo), _ang(ang), _ind(ind)
{
}