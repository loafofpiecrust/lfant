
include(../external.pro)

TEMPLATE = lib
CONFIG += dll plugin
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH = ./include

LIBS += -lgl -lglu

DEFINES = GLEW_DLL


SOURCES += \
	src/glewinfo.c \
	src/glew.c

HEADERS += \
	include/GL/wglew.h \
	include/GL/glxew.h \
	include/GL/glew.h

