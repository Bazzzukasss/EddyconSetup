#-------------------------------------------------
#
# Project created by QtCreator 2014-11-25T15:39:23
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = EddyconSetup
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    UDPClient_EddyconC.cpp \
    UDPClient.cpp \
    qcustomplot.cpp \
    TNetDevice.cpp

HEADERS  += mainwindow.h \
    UDPClient.h \
    UDPClient_EddyconC.h \
    HWNetPacket.hpp \
    qcustomplot.h \
    HWNetSets.h \
    HWSets.h \
    TNetDevice.h \
    TDataSrc.h \
    TThread.h

FORMS    += mainwindow.ui
