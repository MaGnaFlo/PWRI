TEMPLATE = app

QT += widgets printsupport

RC_FILE = pwri.rc

INCLUDEPATH += C:\opencv\release2\install\include
LIBS += "C:/opencv/release2/bin/*.dll"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PWRi

SOURCES += \
    main.cpp \
    videomanager.cpp \
    operation.cpp \
    affinetransform.cpp \
    featuredetector.cpp \
    qcustomplot.cpp \
    mainwindow.cpp \
    calibrationfilemanager.cpp \
    dataanalyzer.cpp \
    spectrumextractor.cpp \
    mediamanager.cpp \
    datafilemanager.cpp

HEADERS += \
    videomanager.h \
    operation.h \
    affinetransform.h \
    featuredetector.h \
    qcustomplot.h \
    mainwindow.h \
    calibrationfilemanager.h \
    dataanalyzer.h \
    spectrumextractor.h \
    mediamanager.h \
    datafilemanager.h

FORMS += \
    mainwindow.ui
