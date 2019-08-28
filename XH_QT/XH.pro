    #-------------------------------------------------
#
# Project created by QtCreator 2017-09-05T08:08:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = XH
TEMPLATE = app


SOURCES += main.cpp\
        xh_test.cpp \
    cancomm.cpp \
    qcustomplot.cpp

HEADERS  += xh_test.h \
    cancomm.h \
    icancmd.h \
    qcustomplot.h

FORMS    += xh_test.ui
