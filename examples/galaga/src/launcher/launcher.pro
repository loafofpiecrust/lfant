include(../galaga.pro)
include($$SFE/src/external/includes.pri)

TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -l:libdl.so

SOURCES += \
	main.cpp
