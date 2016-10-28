#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T10:14:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FTCScout
TEMPLATE = app


SOURCES += Sources/main.cpp\
        Sources/mainwindow.cpp \
    Sources/team.cpp \
    Sources/drawpath.cpp

HEADERS  += Headers/mainwindow.h \
    Headers/team.h \
    Headers/drawpath.h

FORMS    += Forms/mainwindow.ui

RESOURCES += \
    Resources/resources.qrc
