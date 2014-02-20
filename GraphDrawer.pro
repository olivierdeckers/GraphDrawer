#-------------------------------------------------
#
# Project created by QtCreator 2014-01-13T16:01:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GraphDrawer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphcanvas.cpp \
    layoutworker.cpp \
    TSA.cpp \
    TSALayout.cpp \
    PlanarityApprox.cpp \
    TSAAttraction.cpp \
    TSARepulsion.cpp \
    TSAPlanarity.cpp \
    qcustomplot.cpp \
    energyplotter.cpp \
    accstructcomparison.cpp \
    tsaplanaritygrid.cpp

HEADERS  += mainwindow.h \
    graphcanvas.h \
    layoutworker.h \
    TSA.h \
    TSALayout.h \
    PlanarityApprox.h \
    TSAAttraction.h \
    TSARepulsion.h \
    TSAPlanarity.h \
    qcustomplot.h \
    energyplotter.h \
    accstructcomparison.h \
    tsaplanaritygrid.h

FORMS    += mainwindow.ui \
    accstructcomparison.ui

LIBS += -L$$PWD/../../OGDF/OGDF/Win32/Debug -lOGDF
LIBS += -L"E:/Program Files/Microsoft SDKs/Windows/v7.1/Lib" -lGdi32

INCLUDEPATH += $$PWD/../../OGDF/OGDF/include

DEFINES += OGDF_DEBUG GRAPHDRAWER
