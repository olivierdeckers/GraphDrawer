#-------------------------------------------------
#
# Project created by QtCreator 2014-01-13T16:01:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GraphDrawer
TEMPLATE = app


SOURCES += gui/main.cpp\
        gui/mainwindow.cpp \
    gui/graphcanvas.cpp \
    gui/layoutworker.cpp \
    gui/qcustomplot.cpp \
    gui/energyplotter.cpp \
    ogdf/TSA.cpp \
    ogdf/TSALayout.cpp \
    ogdf/PlanarityApprox.cpp \
    ogdf/TSAAttraction.cpp \
    ogdf/TSARepulsion.cpp \
    ogdf/TSAPlanarity.cpp \
    gui/accstructcomparison.cpp \
    ogdf/tsaplanaritygrid.cpp \
    ogdf/tsauniformgrid.cpp

HEADERS  += gui/mainwindow.h \
    gui/graphcanvas.h \
    gui/layoutworker.h \
    ogdf/TSA.h \
    ogdf/TSALayout.h \
    ogdf/PlanarityApprox.h \
    ogdf/TSAAttraction.h \
    ogdf/TSARepulsion.h \
    ogdf/TSAPlanarity.h \
    gui/qcustomplot.h \
    gui/energyplotter.h \
    gui/accstructcomparison.h \
    ogdf/tsaplanaritygrid.h \
    ogdf/tsauniformgrid.h

FORMS    += gui/mainwindow.ui \
    gui/accstructcomparison.ui

win32 {
LIBS += -L$$PWD/../../OGDF/OGDF/Win32/Debug -lOGDF
LIBS += -L"E:/Program Files/Microsoft SDKs/Windows/v7.1/Lib" -lGdi32

INCLUDEPATH += $$PWD/../../OGDF/OGDF/include
}
unix {
LIBS += -L$$PWD/../../OGDF/_debug -lOGDF
INCLUDEPATH += $$PWD/../../OGDF

QMAKE_CXXFLAGS += -std=c++11
}
DEFINES += OGDF_DEBUG GRAPHDRAWER
