#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24T20:24:43
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCPClientWithGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientsocket.cpp

HEADERS  += mainwindow.h \
    clientsocket.h

FORMS    += mainwindow.ui
