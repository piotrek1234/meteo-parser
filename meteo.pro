#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T12:46:36
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = meteo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    meteo.cpp \
    filedownloader.cpp \
    textrecognizer.cpp

HEADERS  += mainwindow.h \
    meteo.h \
    filedownloader.h \
    textrecognizer.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
