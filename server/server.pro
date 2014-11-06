#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T22:31:23
#
#-------------------------------------------------

QT       += core gui
QT       += network

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    SslServer.cpp \
    chatrecordform.cpp

HEADERS  += mainwindow.h \
    SslServer.h \
    chatrecordform.h

FORMS    += mainwindow.ui \
    chatrecordform.ui
