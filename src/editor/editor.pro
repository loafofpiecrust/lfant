#-------------------------------------------------
#
# Project created by QtCreator 2014-05-07T03:43:34
#
#-------------------------------------------------

QT       += core gui

QMAKE_CC = clang
QMAKE_CXX = clang++

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = editor
TEMPLATE = app

SOURCES += *.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

INCLUDEPATH += ../ ../../external/glm ../../external/boost

QMAKE_CXXFLAGS += -std=gnu++11
