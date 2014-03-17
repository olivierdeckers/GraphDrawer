QT -= gui

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += tsauniformgridtest.cpp \
    ../GraphDrawer/ogdf/TSAUniformGrid.cpp \
    ../GraphDrawer/ogdf/TSARepulsion.cpp \
    ../GraphDrawer/ogdf/TSAPlanarityGrid.cpp \
    ../GraphDrawer/ogdf/TSAPlanarity.cpp \
    ../GraphDrawer/ogdf/TSALayout.cpp \
    ../GraphDrawer/ogdf/TSAAttraction.cpp \
    ../GraphDrawer/ogdf/TSA.cpp \
    ../GraphDrawer/ogdf/PlanarityApprox.cpp \
    ../GraphDrawer/ogdf/TSAAngularResolution.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix {
LIBS += -L$$PWD/../../OGDF/_debug -lOGDF -lgtest -lpthread
INCLUDEPATH += $$PWD/../../OGDF
INCLUDEPATH += $$PWD/../GraphDrawer

QMAKE_CXXFLAGS += -std=c++11
}

HEADERS += \
    ../GraphDrawer/ogdf/TSAUniformGrid.h \
    ../GraphDrawer/ogdf/TSARepulsion.h \
    ../GraphDrawer/ogdf/TSAPlanarityGrid.h \
    ../GraphDrawer/ogdf/TSAPlanarity.h \
    ../GraphDrawer/ogdf/TSALayout.h \
    ../GraphDrawer/ogdf/TSAAttraction.h \
    ../GraphDrawer/ogdf/TSA.h \
    ../GraphDrawer/ogdf/PlanarityApprox.h \
    ../GraphDrawer/ogdf/TSAAngularResolution.h
