#-------------------------------------------------
#
# Project created by QtCreator 2020-05-07T23:00:20
#
#-------------------------------------------------
LIBS += -lpng -lm
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mainwindow4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pngimage.cpp

HEADERS  += mainwindow.h \
    pngimage.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

CONFIG += warn_off
