#-------------------------------------------------
#
# Project created by QtCreator 2014-01-13T16:01:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphDrawer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphcanvas.cpp \
    layoutworker.cpp \
    TSA.cpp \
    TSALayout.cpp \
    Planarity.cpp \
    PlanarityApprox.cpp

HEADERS  += mainwindow.h \
    graphcanvas.h \
    layoutworker.h \
    TSA.h \
    TSALayout.h \
    Planarity.h \
    PlanarityApprox.h

FORMS    += mainwindow.ui

LIBS += -L$$PWD/../../OGDF/OGDF/Win32/Debug -lOGDF
LIBS += -L"E:/Program Files/Microsoft SDKs/Windows/v7.1/Lib" -lGdi32

INCLUDEPATH += $$PWD/../../OGDF/OGDF/include

DEFINES += OGDF_DEBUG
