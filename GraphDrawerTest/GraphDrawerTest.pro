QT -= gui

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += tsauniformgridtest.cpp \
    ../GraphDrawer/ogdf/TSAUniformGrid.cpp \
    ../GraphDrawer/ogdf/TSARepulsion.cpp \
    ../GraphDrawer/ogdf/TSAPlanarity.cpp \
    ../GraphDrawer/ogdf/TSALayout.cpp \
    ../GraphDrawer/ogdf/TSAAttraction.cpp \
    ../GraphDrawer/ogdf/TSA.cpp \
    ../GraphDrawer/ogdf/PlanarityApprox.cpp \
    ../GraphDrawer/ogdf/TSAAngularResolution.cpp \
    ../GraphDrawer/ogdf/TSANoAcceleration.cpp \
    ../GraphDrawer/ogdf/RemoveCrossing.cpp \
    ../GraphDrawer/ogdf/RandomMove.cpp \
    ../GraphDrawer/ogdf/TSAEnergyFunction.cpp \
    ../GraphDrawer/ogdf/TSANodePairEnergy.cpp \
    ../GraphDrawer/ogdf/AccelerationStructure.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix {
LIBS += -L$$PWD/../../OGDF/_debug -lOGDF -lgtest -lpthread
INCLUDEPATH += $$PWD/../../OGDF
INCLUDEPATH += $$PWD/../GraphDrawer

QMAKE_CXXFLAGS += -std=c++11 -DOGDF_DEBUG
}

HEADERS += \
    ../GraphDrawer/ogdf/TSAUniformGrid.h \
    ../GraphDrawer/ogdf/TSARepulsion.h \
    ../GraphDrawer/ogdf/TSAPlanarity.h \
    ../GraphDrawer/ogdf/TSALayout.h \
    ../GraphDrawer/ogdf/TSAAttraction.h \
    ../GraphDrawer/ogdf/TSA.h \
    ../GraphDrawer/ogdf/PlanarityApprox.h \
    ../GraphDrawer/ogdf/TSAAngularResolution.h \
    ../GraphDrawer/ogdf/RemoveCrossing.h \
    ../GraphDrawer/ogdf/RandomMove.h \
    ../GraphDrawer/ogdf/NeighbourhoodStructure.h \
    ../GraphDrawer/ogdf/TSAEnergyFunction.h \
    ../GraphDrawer/ogdf/TSANodePairEnergy.h \
    ../GraphDrawer/ogdf/TSANoAcceleration.h \
    ../GraphDrawer/ogdf/AccelerationStructure.h
