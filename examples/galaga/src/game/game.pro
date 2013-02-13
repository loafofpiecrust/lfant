include(../galaga.pro)
include($${SFE}/src/external/includes.pri)

CONFIG += dll plugin
CONFIG -= app_bundle
CONFIG -= qt
TEMPLATE = lib

TARGET = galaga
LIBS += -lsfengine

HEADERS = \
	Projectile.hpp \
	Player.hpp \
	Galaga.hpp

SOURCES = \
	Projectile.cpp \
	Player.cpp \
	Galaga.cpp
