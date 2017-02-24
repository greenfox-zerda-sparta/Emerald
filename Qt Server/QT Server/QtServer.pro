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
    catch.hpp \
    Device.h \
    MessageConverter.h \
    UI.h \
    SubDevice.h \
    DeviceLogfile.h \
    MessageLogfile.h \
    Commands.h \
    Messages.h \
    Server.h \
    UDPSender.h

SOURCES = \
    main.cpp \
    MessageHandler.cpp \
    MyTime.cpp \
    Device.cpp \
    MessageConverter.cpp \
    test.cpp \
    UI.cpp \
    SubDevice.cpp \
    DeviceLogfile.cpp \
    MessageLogfile.cpp \
    Commands.cpp \
    Messages.cpp \
    Server.cpp \
    UDPSender.cpp

INCLUDEPATH = \
    $$PWD/.

#DEFINES = 

