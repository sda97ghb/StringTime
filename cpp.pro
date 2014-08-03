TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += c++11

QMAKE_CXXFLAGS+= -std=c++11
QMAKE_LFLAGS +=  -std=c++11

SOURCES += main.cpp \
    StringTime.cpp

HEADERS += \
    StringTime.h
