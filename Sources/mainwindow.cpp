#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <QVector>
#include <stdio.h>
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
#include <iostream>
#include <vector>
#include <algorithm>
#include "videomanager.h"
#include "affinetransform.h"
#include <QApplication>
#include <string>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->firstNumber->setDisabled(true);
    ui->lastNumber->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeStep()
{
    double step = (ui->angleEnd->value() - ui->angleStart->value())/(double)(_imgEnd - _imgStart);
    std::ostringstream strs;
    strs << step;
    std::string step_s = strs.str();
    ui->angleStep->setText(QString::fromStdString(step_s));
}

void MainWindow::makePlot()
{
    int n_images = 200;

    ui->customPlot->addGraph();
    ui->customPlot->xAxis->setLabel("Images");
    ui->customPlot->yAxis->setLabel("Spectre");

    ui->customPlot->xAxis->setRange(0,n_images);
    ui->customPlot->yAxis->setRange(150,220);

    ui->customPlot->graph(0)->setPen(QPen(Qt::red));

    QVector<double> x(n_images), y(n_images);

    for (int i = 0; i < n_images; i++)
    {
        Mat image;
        stringstream ss;
        ss << i;
        string is = ss.str();
        string path = "C:/Users/Florian/Documents/Travail/Supoptique/3A/Projet PWRi/data/20151105-plasmon water/10x/";

        // Read the file
        if (i<10)
            image = imread(path + "eau 2015_11_05_181528,785 Image 00" + is +".png", IMREAD_COLOR);
        else if (i<100)
            image = imread(path + "eau 2015_11_05_181528,785 Image 0" + is + ".png", IMREAD_COLOR);
        else
            image = imread(path + "eau 2015_11_05_181528,785 Image " + is + ".png", IMREAD_COLOR);

        if (!image.data) // Check for invalid input
        {
            std::cout << "Could not open or find the image" << std::endl;
        }

        Scalar tscal = mean(image);
        float meanIm = tscal.val[0];
        y[i] = meanIm;
        x[i] = i;

        ui->customPlot->graph(0)->setData(x,y);
        ui->customPlot->replot();
        ui->progressBar->setValue((int)i/2 + 1);
    }
}

void MainWindow::on_afficheSpectre_clicked()
{
    ui->progressBar->show();
    MainWindow::makePlot();
    ui->progressBar->hide();
}

void MainWindow::on_createVideo_clicked()
{
    videoManager vm(_pathS, _ext, 20, "", "video");
    vm.createVideo();
}

void MainWindow::on_preproc_clicked()
{
    affineTransform aff(_imgStart, 5, _pathR, _name, _ext);
    aff.process();
}

void MainWindow::on_pushSelectImg_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isEmpty())
    {
        string s_fileNames = fileName.toLocal8Bit().constData();
        _op.readFileName(s_fileNames, _pathR, _name, _imgStart_ABS, _ext);
        QString Qs_path = QString::fromStdString(_pathR);
        _imgEnd_ABS = _op.findlastImage(_pathR, _name, _ext, _imgStart_ABS);
        _imgStart = _imgStart_ABS;
        _imgEnd = _imgEnd_ABS;

        ui->line_load->setText(Qs_path);
        ui->firstNumber->setEnabled(true);
        ui->lastNumber->setEnabled(true);
        ui->firstNumber->setMaximum(_imgEnd_ABS);
        ui->lastNumber->setValue(_imgEnd_ABS);
        ui->firstNumber->setValue(_imgStart_ABS);
        ui->angleStart->setValue(_imgStart);
        ui->angleEnd->setValue(_imgEnd);
        ui->nImages->setText(QString::fromStdString(_op.int2string(ui->lastNumber->value()+1)));

        changeStep();

    }
}

void MainWindow::on_pushSaveImg_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, tr("Save Directory"), _pathR.c_str(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!folderName.isEmpty())
    {
        folderName += "/";
        ui->line_save->setText(folderName);
    }
}

void MainWindow::on_firstNumber_valueChanged(int arg1)
{
    if(arg1 > ui->lastNumber->value())
        QMessageBox::warning(this, tr("Invalid numbers"), tr("The first image number must be less than the last image number !"));
    _imgStart = arg1;
    changeStep();
}

void MainWindow::on_lastNumber_valueChanged(int arg1)
{
    if(arg1 < ui->firstNumber->value())
        QMessageBox::warning(this, tr("Invalid numbers"), tr("The first image number must be less than the last image number !"));
    _imgEnd = arg1;
    changeStep();
}

void MainWindow::on_angleStart_valueChanged(double arg1)
{
    changeStep();
}

void MainWindow::on_angleEnd_valueChanged(double arg1)
{
    changeStep();
}
