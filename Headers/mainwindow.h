#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QVector>

#include <cstdlib>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <ctime>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv_modules.hpp>

#include "operation.h"
#include "dataanalyzer.h"
#include "affinetransform.h"
#include "calibrationfilemanager.h"
#include "ui_mainwindow.h"
#include "videomanager.h"
#include "spectrumextractor.h"
#include "datafilemanager.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void changeStep();
    void setCalibrationMatrix();
    void setSpectre();

    void setAngleStart(double angleStart);
    void setAngleStep(double angleStep);


private slots:
    void makePlot();
    void on_afficheSpectre_clicked();
    void on_createVideo_clicked();
    void on_preproc_clicked();
    void on_pushSelectImg_clicked();
    void on_pushSaveImg_clicked();
    void on_firstNumber_valueChanged(int arg1);
    void on_lastNumber_valueChanged(int arg1);
    void on_angleStart_valueChanged(double arg1);
    void on_angleEnd_valueChanged(double arg1);
    void on_imageCalibButton_clicked();
    void on_calibrateButton_clicked();
    void on_line_lens_textEdited(const QString &arg1);
    void on_line_wavelength_textEdited(const QString &arg1);
    void on_line_angle_textEdited(const QString &arg1);
    void on_line_index_textEdited(const QString &arg1);
    void on_getExtrema_clicked();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void on_specificCalibration_clicked();
    void on_SaveSpectrum_clicked();
    void on_checkBox_specificCalib_stateChanged(int arg1);

    void on_line_saveSpecific_editingFinished();

    void on_line_loadSpecCalib_editingFinished();

    void on_btn_testSpectrePixel_clicked();

    void on_btn_SaveRawData_clicked();

private:
    Ui::MainWindow *ui;
    affineTransform *_aff;
    CalibrationFileManager _cfm;
    dataAnalyzer _dataAnalyzer;
    SpectrumExtractor *_spect;
    Operation _op;
    string _pathR;
    string _pathS;
    string _name;
    string _ext;
    int _imgStart_ABS;
    int _imgEnd_ABS;
    int _imgEnd;
    int _imgStart;
    double _angleStart;
    double _angleStep;
    //vector<double> _dataY;
    //vector<double> _dataX;
    vector<int> _minVect;
    int _maxVect;
    QRubberBand *_rubberband;
    QPoint _startPoint;
    QPoint _currentPoint;
};

#endif // MAINWINDOW_H
