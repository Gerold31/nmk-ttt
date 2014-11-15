#-------------------------------------------------
#
# Project created by QtCreator 2014-11-14T15:13:28
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = nmk-ttt
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    nmk.cpp \
    game.cpp \
    gamelocal.cpp \
    gameclient.cpp \
    gameserver.cpp \
    gameserverclient.cpp

HEADERS += \
    nmk.h \
    game.h \
    gamelocal.h \
    gameclient.h \
    gameserver.h \
    gameserverclient.h
