
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}





