# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT = core
QT += network
QT -= gui

CONFIG += c++11

TARGET = QtServer
CONFIG += console

TEMPLATE = app

HEADERS = \
    MessageHandler.h \
    MyTime.h \
    server.h \
    udpsender.h \
    catch.hpp \
    Device.h \
    MessageConverter.h \
    UI.h \
    messages.h \
    commands.h \
    CommandMap.h \
    SubDevice.h \
    DeviceLogfile.h \
    MessageLogfile.h

SOURCES = \
    main.cpp \
    MessageHandler.cpp \
    MyTime.cpp \
    server.cpp \
    udpsender.cpp \
    Device.cpp \
    MessageConverter.cpp \
    test.cpp \
    UI.cpp \
    messages.cpp \
    commands.cpp \
    SubDevice.cpp \
    DeviceLogfile.cpp \
    MessageLogfile.cpp

INCLUDEPATH = \
    $$PWD/.

#DEFINES = 

