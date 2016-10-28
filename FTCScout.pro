#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T10:14:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FTCScout
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    team.cpp \
    drawpath.cpp

HEADERS  += mainwindow.h \
    team.h \
    drawpath.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
