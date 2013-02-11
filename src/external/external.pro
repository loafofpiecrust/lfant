TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = glew \
	glfw \
    boost

# ShadowFox Engine directory; Change this to your own
SFE = /home/taylorsnead/ShadowFox/ShadowFox-Engine
ROOT = $$SFE

include($$SFE/src/settings.pri)
