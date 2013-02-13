TEMPLATE = subdirs

SUBDIRS = \
game \
launcher

# Must use absolute path for QMake subdirs, set this to your own
SFE = /home/taylorsnead/ShadowFox/ShadowFox-Engine
ROOT = $${SFE}/examples/galaga

include($${SFE}/build.pri)

# Project Options
INCLUDEPATH += $${SFE}/src/engine
LIBS += -L$${SFE}/bin$${ARCH}/$${PLATFORM}

OTHER_FILES = \
../assets/shaders/Diffuse.vert \
../assets/shaders/Diffuse.frag
