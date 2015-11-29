#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "operation.h"

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

private:
    Ui::MainWindow *ui;
    Operation _op;
    string _pathR;
    string _pathS;
    string _name;
    string _ext;
    int _imgStart_ABS;
    int _imgEnd_ABS;
    int _imgEnd;
    int _imgStart;
};

#endif // MAINWINDOW_H
