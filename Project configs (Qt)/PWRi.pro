CONFIG += release
CONFIG += debug
TEMPLATE = app

TARGET = PWRi


INCLUDEPATH += C:\opencv\release2\install\include
LIBS += "C:/opencv/release2/bin/*.dll"


SOURCES += \
    main.cpp \
    videomanager.cpp \
    operation.cpp \
    affinetransform.cpp \
    featuredetector.cpp

HEADERS += \
    videomanager.h \
    operation.h \
    affinetransform.h \
    featuredetector.h
