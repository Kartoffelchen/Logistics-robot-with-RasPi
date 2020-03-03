#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T19:50:03
#
#-------------------------------------------------

QT += core gui
QT += xml
QT += xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WallE
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    abstractsensor.cpp \
    dcmotor.cpp \
    encoder.cpp \
    linesensor.cpp \
    ultraschallsensor.cpp \
    mobileplatform.cpp \
    controlsystem.cpp \
    xmlparsen.cpp \
    ledcontrol.cpp

HEADERS  += mainwindow.h \
    abstractsensor.h \
    dcmotor.h \
    encoder.h \
    linesensor.h \
    ultraschallsensor.h \
    mobileplatform.h \
    controlsystem.h \
    namesanddefines.h \
    xmlparsen.h \
    ledcontrol.h

FORMS    += mainwindow.ui

LIBS += -lwiringPi -lpthread
