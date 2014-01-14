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
    layoutworker.cpp

HEADERS  += mainwindow.h \
    graphcanvas.h \
    layoutworker.h

FORMS    += mainwindow.ui

LIBS += -L$$PWD/../../OGDF/OGDF/Win32/Debug -lOGDF

INCLUDEPATH += $$PWD/../../OGDF/OGDF/include
