
include(../external.pro)

TEMPLATE = lib
CONFIG += dll plugin
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH = ./include ./src

DEFINES = GLFW_BUILD_DLL
linux:DEFINES += _GLFW_HAS_PTHREAD _GLFW_HAS_DLOPEN _GLFW_HAS_SYSCONF _GLFW_HAS_SYSCTL _GLFW_HAS_GLXGETPROCADDRESS

linux:LIBS += -lglx -lX11

SOURCES += \
	src/window.c \
	src/time.c \
	src/thread.c \
	src/tga.c \
	src/stream.c \
	src/main.cpp \
	src/joystick.c \
	src/input.c \
	src/init.c \
	src/image.c \
	src/glext.c \
	src/fullscreen.c \
	src/enable.c \

linux:SOURCES += \
	src/x11/x11_window.c \
	src/x11/x11_time.c \
	src/x11/x11_thread.c \
	src/x11/x11_keysym2unicode.c \
	src/x11/x11_joystick.c \
	src/x11/x11_init.c \
	src/x11/x11_glext.c \
	src/x11/x11_fullscreen.c \
	src/x11/x11_enable.c

win32:SOURCES += \
	src/win32/win32_window.c \
	src/win32/win32_time.c \
	src/win32/win32_thread.c \
	src/win32/win32_joystick.c \
	src/win32/win32_init.c \
	src/win32/win32_glext.c \
	src/win32/win32_fullscreen.c \
	src/win32/win32_enable.c \
	src/win32/win32_dllmain.c

macx:SOURCES += \
	src/cocoa/cocoa_thread.c

HEADERS += \
	src/platform.h \
	src/internal.h \
	include/GL/glfw.h \
	src/x11/platform.h \
	src/win32/platform.h \
	src/cocoa/platform.h

macx:OTHER_FILES += \
	src/cocoa/cocoa_window.m \
	src/cocoa/cocoa_time.m \
	src/cocoa/cocoa_joystick.m \
	src/cocoa/cocoa_init.m \
	src/cocoa/cocoa_glext.m \
	src/cocoa/cocoa_fullscreen.m \
	src/cocoa/cocoa_enable.m

