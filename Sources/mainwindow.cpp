#include "mainwindow.h"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cout << "Avant setup" << endl;
    ui->setupUi(this);
    cout << "Apres setup" << endl;
    ui->spectreBar->hide();
    ui->preprocBar->hide();
    ui->calibBar->hide();
    ui->firstNumber->setDisabled(true);
    ui->lastNumber->setDisabled(true);
    _aff = new affineTransform();
    _spect = new SpectrumExtractor();
    ui->tabWidget->setCurrentIndex(0);
    ui->customPlot->setEnabled(false);

    //Parameters setup
    ui->line_lens->setText("4");
    ui->line_wavelength->setText("400");
    ui->line_angle->setText("45");
    ui->line_index->setText("1.5");
    setCalibrationMatrix();
    ui->radioButton_defaultParameters->click();

    //Tool Tips
    ui->pushSelectImg->setToolTip("Please select the 1st image of the set you want to process.");
    ui->pushSaveImg->setToolTip("Please select the folder where you want to save the processed images.\nThe processed images are the nondeformed images." );
    ui->angleStart->setToolTip("Between -360° & +360°");
    ui->angleEnd->setToolTip("Between -360° & +360°");
    ui->line_lens->setToolTip("Here you can specify the lens magnification you are using.");
    ui->line_index->setToolTip("Here you can specify the index of the prism you are using.");
    ui->line_wavelength->setToolTip("Here you can specify the wavelength used.");
    ui->line_angle->setToolTip("Here you can specify the top angle of the prism.");
    ui->imageCalibButton->setToolTip("If needed, you can here select the first image of your calibration set of images.");
    ui->calibrateButton->setToolTip("Use this button if you need to calibrate your system\n(in case the calibration you are using does not exist yet). \nThis operation may take a very long time depending on how many images you are using to calibrate.");
    ui->preproc->setToolTip("This launches the pre-processing operation. \nBy clicking this button, all the images will be calibrated with the calibration file you chose to use. \nAll the calibrated images will be saved in the Save directory you specified at the beginning. \nBe patient, this operation may take a while");
    ui->afficheSpectre->setToolTip("Show the current spectrum. \nPlease use the pre-processing button before using this action.");
    ui->getExtrema->setToolTip("This allows you to see where the minima are located. \nUse this after the pre-processing operation.");

    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), ui->customPlot, SLOT(showPointToolTip(QMouseEvent*)));

    _rubberband = new QRubberBand(QRubberBand::Rectangle, this);

    //
    ui->spectreBar->setStyleSheet("QProgressBar {background: blue;}");
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

void MainWindow::setCalibrationMatrix()
{
    int obj, ldo, ang;
    double ind;

    string s = ui->line_lens->text().toLocal8Bit().constData();
    obj = atoi(s.c_str());

    s = ui->line_wavelength->text().toLocal8Bit().constData();
    ldo = atoi(s.c_str());

    s = ui->line_angle->text().toLocal8Bit().constData();
    ang = atoi(s.c_str());

    s = ui->line_index->text().toLocal8Bit().constData();
    ind = atof(s.c_str());

    _cfm = CalibrationFileManager(obj, ldo, ang, ind);

    bool isStandard = !ui->checkBox_specificCalib->isChecked(); // false if using a specific calibration file
    _cfm.setIsStandard(isStandard);
    _cfm.setName(ui->line_saveSpecific->text().toStdString());

    _cfm.loadFile();
    if(_cfm.isOpen())
    {
        _aff->setMeanTransform(_cfm.calibMat());
        cout << "MainWindow::setCalibrationMatrix : " << _aff->meanTransform() << endl;
        cout << "MainWindow::setCalibrationMatrix : " << _cfm.calibMat() << endl;
        ui->label_isCalibOK->setText("Calibration file exists !");
        ui->label_isCalibOK->setStyleSheet("QLabel { color : green; }");
    }
    else
    {
        ui->label_isCalibOK->setText("Calibration file doesn't exist. Please calibrate the system first.");
        ui->label_isCalibOK->setStyleSheet("QLabel { color : red; }");
        _aff->setMeanTransform(Mat::eye(2,3,CV_64F ));
    }
}

void MainWindow::setSpectre()
{
    delete _spect;
    _spect = new SpectrumExtractor(_imgStart, _imgEnd, _angleStart, _angleStep, _pathS, _name, _ext);
    ui->spectreBar->show();
    _spect->processSpectrumAverage(ui->spectreBar);
    ui->spectreBar->hide();
}

void MainWindow::makePlot()
{
    vector<double> dataX = _spect->dataX();
    vector<double> dataY = _spect->dataY();
    if(dataX.size() != dataY.size())
    {
        cout << "MainWindow::makePlot : Caution ! X and Y don't have the same size !" << endl;
        return;
    }
    if(dataX.size() == 0 || dataY.size() == 0)
    {
        cout << "MainWindow::makePlot : Caution ! X and/or Y are empty !" << endl;
        return;
    }

    int n_images = dataY.size();

    ui->customPlot->addGraph();
    ui->customPlot->xAxis->setLabel("Angle");
    ui->customPlot->yAxis->setLabel("Spectre");
    ui->customPlot->xAxis->setRange(_angleStart,_angleStart + n_images*_angleStep);

    double m, M;
    _op.minAndMaxVectorDouble(dataY, m, M);
    double range = (m+M)/20.;
    cout << "MainWindow::makePlot : m-range " << m-range << " and M+range " << M+range << endl;
    ui->customPlot->yAxis->setRange(m-range, M+range);
    ui->customPlot->graph(0)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setData(QVector<double>::fromStdVector(dataX),QVector<double>::fromStdVector(dataY));
    ui->customPlot->replot();
}

void MainWindow::on_afficheSpectre_clicked()
{
    ui->customPlot->setEnabled(true);
    setSpectre();
    makePlot();
}

void MainWindow::on_createVideo_clicked()
{
    videoManager vm(_pathS, _ext, 20, "Processed_" + _name, "video");
    vm.saveFile();
}

void MainWindow::on_preproc_clicked()
{
    this->repaint();
    _aff->setImgStart(_imgStart);
    _aff->setImgEnd(_imgEnd);
    _aff->setPath(_pathR);
    _aff->setName(_name);
    _aff->setExt(_ext);
    _aff->setMidXY();
    ui->preprocBar->show();
    _aff->createWarpedImages_it(_pathS, ui->preprocBar);
    ui->preprocBar->hide();

    setSpectre();

    cout << "MainWindow::on_preproc_clicked : Setting spectre -> Creation of dataX and dataY." << endl;
}

void MainWindow::on_pushSelectImg_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isEmpty())
    {
        string s_fileNames = fileName.toLocal8Bit().constData();
        _op.readFileName(s_fileNames, _pathR, _name, _imgStart_ABS, _ext, true);
        QString Qs_path = QString::fromStdString(_pathR);
        _imgEnd_ABS = _op.findlastImage(_pathR, _name, _ext, _imgStart_ABS);
        _imgStart = _imgStart_ABS;
        _imgEnd = _imgEnd_ABS;

        ui->line_load->setText(Qs_path);
        ui->firstNumber->setEnabled(true);
        ui->lastNumber->setEnabled(true);

        ui->firstNumber->setMaximum(_imgEnd_ABS);
        ui->firstNumber->setMinimum(_imgStart_ABS);
        ui->lastNumber->setMaximum(_imgEnd_ABS);
        ui->lastNumber->setMinimum(_imgStart_ABS);

        ui->lastNumber->setValue(_imgEnd_ABS);
        ui->firstNumber->setValue(_imgStart_ABS);
        ui->angleStart->setValue(_imgStart);
        ui->angleEnd->setValue(_imgEnd);

        ui->firstNumber->setToolTip(QString::fromStdString("Between " + _op.int2string(_imgStart_ABS) + " & " + _op.int2string(_imgEnd_ABS)));
        ui->lastNumber->setToolTip(QString::fromStdString("Between " + _op.int2string(_imgStart_ABS) + " & " + _op.int2string(_imgEnd_ABS)));

        ui->nImages->setText(QString::fromStdString(_op.int2string(ui->lastNumber->value()+1)));
        changeStep();
        setAngleStart(ui->angleStart->value());
        setAngleStep(atof(ui->angleStep->text().toLocal8Bit().constData()));

        _aff->setImgStart(_imgStart);
        _aff->setImgEnd(_imgEnd);
        _aff->setPath(_pathR);
        _aff->setName(_name);
        _aff->setExt(_ext);
        _aff->setMidXY();
    }
}

void MainWindow::on_pushSaveImg_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, tr("Save Directory"), _pathR.c_str(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!folderName.isEmpty())
    {
        folderName += "/";
        ui->line_save->setText(folderName);
        _pathS = folderName.toLocal8Bit().constData();
    }
    cout << "MainWindow::on_pushSaveImg_clicked : pathS = " << _pathS << endl;
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
    setAngleStart(ui->angleStart->value());
    setAngleStep(atof(ui->angleStep->text().toLocal8Bit().constData()));
    (void)arg1;
}

void MainWindow::on_angleEnd_valueChanged(double arg1)
{
    changeStep();
    setAngleStart(ui->angleStart->value());
    setAngleStep(atof(ui->angleStep->text().toLocal8Bit().constData()));
    (void)arg1;
}

void MainWindow::on_imageCalibButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", tr("Image Files (*.png *.jpg *.bmp)"));
    int imgCalStart;
    int imgCalEnd;
    string path;
    string ext;
    string name;

    if(!fileName.isEmpty())
    {
        string s_fileNames = fileName.toLocal8Bit().constData();
        _op.readFileName(s_fileNames, path, name, imgCalStart, ext, true);
        QString Qs_path = QString::fromStdString(path);
        imgCalEnd = _op.findlastImage(path, name, ext, imgCalStart);
        _aff->setImgStart(imgCalStart);
        _aff->setImgEnd(imgCalEnd);
        _aff->setPath(path);
        _aff->setName(name);
        _aff->setExt(ext);
        _aff->setMidXY();
        ui->line_imgCalib->setText(Qs_path);
        ui->stateLabel->setText("Click to calibrate (This may take a while)");
    }
}

void MainWindow::on_calibrateButton_clicked()
{
    if(ui->line_imgCalib->text().size() == 0)
    {
        QMessageBox::warning(this, tr("No calibration set selected !"), tr("Please select a calibration images set first"));
    }
    else if(ui->radioButton_specificName->isChecked() && ui->line_saveSpecific->text().size() == 0)
    {
        QMessageBox::warning(this, tr("No file name selected !"), tr("Please select a file name first"));
    }
    else
    {
        ui->calibBar->show();
        _aff->process(ui->calibBar);
        ui->calibBar->hide();
        string s = ui->line_lens->text().toLocal8Bit().constData();
        int obj = atoi(s.c_str());

        s = ui->line_wavelength->text().toLocal8Bit().constData();
        int ldo = atoi(s.c_str());

        s = ui->line_angle->text().toLocal8Bit().constData();
        int ang = atoi(s.c_str());

        s = ui->line_index->text().toLocal8Bit().constData();
        double ind = atof(s.c_str());

        _cfm.getSetup().setObj(obj);
        _cfm.getSetup().setLdo(ldo);
        _cfm.getSetup().setAng(ang);
        _cfm.getSetup().setInd(ind);
        _cfm.setCalibMat(_aff->meanTransform());
        _cfm.setIsStandard(!ui->radioButton_specificName->isChecked());
        string name = ui->radioButton_specificName->isChecked()?ui->line_saveSpecific->text().toStdString():"";
        _cfm.setName(name);

        _cfm.saveFile();

        ui->stateLabel->setText("Calibration complete !");
    }
    if(ui->radioButton_specificName->isChecked())
    {
        ui->checkBox_specificCalib->click();
        ui->line_loadSpecCalib->setText(QString::fromStdString(_cfm.fullPath()));
    }
    setCalibrationMatrix();
}

void MainWindow::on_line_lens_textEdited(const QString &arg1)
{
    string s_obj = arg1.toLocal8Bit().constData();
    _cfm.getSetup().setObj(atoi(s_obj.c_str()));
    setCalibrationMatrix();
}

void MainWindow::on_line_wavelength_textEdited(const QString &arg1)
{
    string s_ldo = arg1.toLocal8Bit().constData();
    _cfm.getSetup().setObj(atoi(s_ldo.c_str()));
    setCalibrationMatrix();
}

void MainWindow::on_line_angle_textEdited(const QString &arg1)
{
    string s_ang = arg1.toLocal8Bit().constData();
    _cfm.getSetup().setObj(atoi(s_ang.c_str()));
    setCalibrationMatrix();
}

void MainWindow::on_line_index_textEdited(const QString &arg1)
{
    string s_ind = arg1.toLocal8Bit().constData();
    _cfm.getSetup().setObj(atof(s_ind.c_str()));
    setCalibrationMatrix();
}

void MainWindow::on_getExtrema_clicked()
{
    int minP, minS;
    _dataAnalyzer.setData(_spect->dataY());
    _dataAnalyzer.process(minP, minS);

    QPixmap pixP(QString::fromStdString(_pathS + "Processed_" + _name + _op.int2string(minP) + _ext));
    QPixmap pixS(QString::fromStdString(_pathS + "Processed_" + _name + _op.int2string(minS) + _ext));
    int w = ui->imageP->width();
    int h = ui->imageS->height();
    ui->imageP->setPixmap(pixP.scaled(w,h,Qt::KeepAspectRatio));
    ui->imageS->setPixmap(pixS.scaled(w,h,Qt::KeepAspectRatio));

    ostringstream ssP, ssS;
    ssP << _angleStart + minP*_angleStep;
    ssS << _angleStart + minS*_angleStep;

    QString p_text = QString::fromStdString("P : Image " + _op.int2string(minP) + " (" + ssP.str() + "°)");
    QString s_text = QString::fromStdString("S : Image " + _op.int2string(minS) + " (" + ssS.str() + "°)");
    ui->P_label->setText(p_text);
    ui->S_label->setText(s_text);

    //Image to do the processing
    ui->resultImage->setPixmap(pixP.scaled(w,h,Qt::KeepAspectRatio));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    _currentPoint = ui->resultImage->mapFromGlobal((this->mapToGlobal(event->pos())));
    int x = _currentPoint.x();
    int y = _currentPoint.y();
    QPoint widgetPos = ui->resultImage->mapFromGlobal((this->mapToGlobal(ui->resultImage->pos())));
    QRect widgetGeometry = ui->resultImage->geometry();

    if(ui->tabWidget->currentIndex() == 3)
        if(x < widgetPos.x() + widgetGeometry.width()
                && x > widgetPos.x()
                && y < widgetPos.y() + widgetGeometry.height()
                && y > widgetPos.y())

        {
            ostringstream sx;
            ostringstream sy;
            _startPoint = ui->resultImage->mapFromGlobal(this->mapToGlobal(event->pos()));
            sx << _startPoint.x();
            ui->x1->setText(QString::fromStdString(sx.str()));
            sy << _startPoint.y();
            ui->y1->setText(QString::fromStdString(sy.str()));

            _rubberband = new QRubberBand(QRubberBand::Rectangle, ui->resultImage);
            _rubberband->setGeometry(QRect(_startPoint, QSize()));
            _rubberband->show();
        }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    _currentPoint = ui->resultImage->mapFromGlobal((this->mapToGlobal(event->pos())));
    int x = _currentPoint.x();
    int y = _currentPoint.y();
    QPoint widgetPos = ui->resultImage->mapFromGlobal((this->mapToGlobal(ui->resultImage->pos())));
    QRect widgetGeometry = ui->resultImage->geometry();

    if(ui->tabWidget->currentIndex() == 3)
        if(x < widgetPos.x() + widgetGeometry.width()
                && x > widgetPos.x() - 1
                && y < widgetPos.y() + widgetGeometry.height()
                && y > widgetPos.y() - 1)
        {
            ostringstream sx;
            ostringstream sy;

            _rubberband->setGeometry(QRect(_startPoint, _currentPoint).normalized());
            sx << ui->resultImage->mapFromGlobal(this->mapToGlobal(event->pos())).x();
            ui->x2->setText(QString::fromStdString(sx.str()));
            sy << ui->resultImage->mapFromGlobal(this->mapToGlobal(event->pos())).y();
            ui->y2->setText(QString::fromStdString(sy.str()));
        }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    (void)event;
    int x = _currentPoint.x();
    int y = _currentPoint.y();
    QPoint widgetPos = ui->resultImage->mapFromGlobal((this->mapToGlobal(ui->resultImage->pos())));
    QRect widgetGeometry = ui->resultImage->geometry();

    if(ui->tabWidget->currentIndex() == 3)
        if(x < widgetPos.x() + widgetGeometry.width()
                && x > widgetPos.x()
                && y < widgetPos.y() + widgetGeometry.height()
                && y > widgetPos.y())
        {
            _rubberband->hide();
            _rubberband->clearMask();

            int xdiff = _rubberband->mapFromGlobal((this->mapToGlobal(_rubberband->pos()))).x() + _rubberband->geometry().width() - widgetPos.x() - widgetGeometry.width();
            int ydiff = _rubberband->mapFromGlobal((this->mapToGlobal(_rubberband->pos()))).y() + _rubberband->geometry().height() - widgetPos.y() - widgetGeometry.height();
            if(xdiff > 0) {
                _rubberband->clearMask();
                /*_rubberband->setGeometry(_rubberband->geometry().x(), _rubberband->geometry().y(),
                                         _rubberband->geometry().width() - xdiff, _rubberband->geometry().height());*/
            }
            if(ydiff > 0)
            _rubberband->setGeometry(_rubberband->geometry().x(), _rubberband->geometry().y(),
                                         _rubberband->geometry().width(), _rubberband->geometry().height() - ydiff - 10);
        }

    _rubberband->clearMask();
    _rubberband->hide();
}

void MainWindow::setAngleStep(double angleStep)
{
    _angleStep = angleStep;
}

void MainWindow::setAngleStart(double angleStart)
{
    _angleStart = angleStart;
}

void MainWindow::on_specificCalibration_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Calibration File (*.txt)"));
    if(!fileName.isEmpty())
    {
        ui->line_loadSpecCalib->setText(fileName);
        _cfm.setName(fileName.toStdString());
        setCalibrationMatrix();
    }
}

void MainWindow::on_SaveSpectrum_clicked()
{
    if(ui->customPlot->isEnabled())
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save spectrum"), QString::fromStdString(_pathS), tr("Image file (*.png)"));
        ui->customPlot->savePng(fileName.toLocal8Bit().constData());
    }
}

void MainWindow::on_checkBox_specificCalib_stateChanged(int arg1)
{
    (void)arg1;
    setCalibrationMatrix();
}

void MainWindow::on_line_saveSpecific_editingFinished()
{
    QString fileName = ui->line_saveSpecific->text();
    if(!fileName.isEmpty())
        _cfm.setName(fileName.toStdString());
}

void MainWindow::on_line_loadSpecCalib_editingFinished()
{
    setCalibrationMatrix();
}

void MainWindow::on_btn_testSpectrePixel_clicked()
{
    ui->spectreBar->show();
    _spect->processSpectrumAllPixels(ui->spectreBar);
    ui->spectreBar->hide();

    Mat minP = _spect->getMinP();
    Mat minS = _spect->getMinS();
    cout << minP.size().height << endl;
    imwrite(_pathS + "min20151214P.png", minP);
    imwrite(_pathS + "min20151214S.png", minS);
}

void MainWindow::on_btn_SaveRawData_clicked()
{
    if(_spect->dataX().empty())
    {
        QMessageBox::warning(this, tr("No spectrum !"), tr("Please compute a spectrum first !"));
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Raw Data"), QString::fromStdString(_pathS), tr("Text file (*.txt)"));
    string path, name, ext;
    int i;
    _op.readFileName(fileName.toStdString(), path, name, i, ext, false);


    DataFileManager dfm(_spect->dataX(), _spect->dataY(), path, name);
    dfm.saveFile();
    cout << dfm.isOpen() << endl;
}
