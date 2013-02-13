
ARCH = 32
!contains(QMAKE_HOST.arch, x86_64)
{
	ARCH = 64
}
PLATFORM = linux
unix:PLATFORM = linux
win32:PLATFORM = windows
macx:PLATFORM = osx

# Compilation options
QMAKE_CFLAGS_RELEASE = -O3 -Os
QMAKE_CXXFLAGS_RELEASE = $${QMAKE_CFLAGS_RELEASE} -std=gnu++11
QMAKE_CXXFLAGS = -m$${ARCH}
QMAKE_CFLAGS_WARN_ON = -Wall -Wfatal-errors
QMAKE_CXXFLAGS_WARN_ON = $${QMAKE_CFLAGS_WARN_ON} -Wno-invalid-offsetof
QMAKE_CXXFLAGS_THREAD =
QMAKE_LFLAGS_RPATH =
QMAKE_LFLAGS_RELEASE =
QMAKE_LFLAGS = -O3 -Wl,-rpath,\'\$$ORIGIN\' -Wl,-z,origin

# Compilation tools
QMAKE_CXX = g++
QMAKE_CC = gcc

# Generic Output
DESTDIR = $${ROOT}/bin$${ARCH}/$${PLATFORM}
DLLDESTDIR = $${DESTDIR}
OBJECTS_DIR = $${DESTDIR}/build/$${TARGET}

# Generic Options
LIBS = -L$${DESTDIR}
