#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T23:32:24
#
#-------------------------------------------------

QT       += core gui
QT       += network

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
