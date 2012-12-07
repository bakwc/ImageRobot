#-------------------------------------------------
#
# Project created by QtCreator 2012-12-06T21:53:36
#
#-------------------------------------------------

QT       += core network

TARGET = image_robot
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    storage.cpp \
    application.cpp \
    process.cpp \
    utils/usettings.cpp \
    utils/ufullscreenwrapper.cpp \
    utils/uexception.cpp \
    utils/ucast.cpp

HEADERS += \
    storage.h \
    application.h \
    process.h \
    utils/usettings.h \
    utils/ufullscreenwrapper.h \
    utils/uexception.h \
    utils/ucast.h

QMAKE_CXXFLAGS += -std=c++0x
