#-------------------------------------------------
#
# Project created by QtCreator 2014-11-29T08:43:55
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LunadllNewLauncher
TEMPLATE = lib

CONFIG += c++11
CONFIG += sharedlib


DEFINES += LunadllNewLauncher


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    setting.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc