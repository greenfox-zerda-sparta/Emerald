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
   $$PWD/Logfile.h \
   $$PWD/MessageHandler.h \
   $$PWD/MyTime.h \
   $$PWD/server.h \
   $$PWD/udpsender.h \
    Alarm.h \
    Blinds.h \
    catch.hpp \
    Cooling.h \
    Device.h \
    GarageDoor.h \
    Heating.h \
    Lamp.h \
    MessageConverter.h \
    resource.h \
    UI.h

SOURCES = \
   $$PWD/Logfile.cpp \
   $$PWD/main.cpp \
   $$PWD/MessageHandler.cpp \
   $$PWD/MyTime.cpp \
   $$PWD/server.cpp \
   $$PWD/udpsender.cpp \
    Alarm.cpp \
    Blinds.cpp \
    Cooling.cpp \
    Device.cpp \
    GarageDoor.cpp \
    Heating.cpp \
    Lamp.cpp \
    MessageConverter.cpp \
    test.cpp \
    UI.cpp

INCLUDEPATH = \
    $$PWD/.

#DEFINES = 

