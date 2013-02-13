include(../galaga.pro)
include($${SFE}/src/external/includes.pri)

CONFIG += dll plugin
CONFIG -= app_bundle
CONFIG -= qt
TEMPLATE = lib

TARGET = galaga
LIBS += -lsfengine
QMAKE_CXXFLAGS += -ggdb

HEADERS = \
    Projectile.hpp \
    Player.hpp \
    Galaga.hpp

SOURCES = \
    Projectile.cpp \
    Player.cpp \
    Galaga.cpp
