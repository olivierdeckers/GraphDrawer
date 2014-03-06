TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += tsauniformgridtest.cpp

unix {
LIBS += -L$$PWD/../../OGDF/_debug -lOGDF -lgtest -lpthread
INCLUDEPATH += $$PWD/../../OGDF

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS += ../build-GraphDrawer-Desktop-Debug/tsauniformgrid.o \
    ../build-GraphDrawer-Desktop-Debug/TSAPlanarity.o
}

