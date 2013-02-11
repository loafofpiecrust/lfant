
include(../sfengine.pro)
include($$SFE/src/external/includes.pri)

TEMPLATE = lib
CONFIG += dll plugin
CONFIG -= app_bundle
CONFIG -= qt

TARGET = sfengine

# Dependency information
INCLUDEPATH += ../external/fmod/api/inc ../external/glm ../external/boost ../external/glew/include ../external/glfw/include ../external/png ../external/bullet/src ../external/raknet/Source
LIBS += -lbullet -lfmod -lboost -lglew -lglfw -lpng -lz -lsquirrel -lraknet

DEFINES = __GXX_EXPERIMENTAL_CXX0X__

SOURCES += \
	Transform.cpp \
	tinyxml2.cpp \
	Time.cpp \
	Thread.cpp \
	TextureLoader.cpp \
	Texture.cpp \
	SystemInfo.cpp \
	Subsystem.cpp \
	StringUtil.cpp \
	Sprite.cpp \
	ShaderLoader.cpp \
	sfRigidBody.cpp \
	sfDynamicsWorld.cpp \
	Settings.cpp \
	Scene.cpp \
	Rigidbody.cpp \
	Renderer.cpp \
	Random.cpp \
	Physics.cpp \
	ParticleSystem.cpp \
	Particle.cpp \
	Object.cpp \
	Network.cpp \
	Mesh.cpp \
	Light.cpp \
	Joint.cpp \
	Input.cpp \
	FileManager.cpp \
	Entity.cpp \
	Engine.cpp \
	Console.cpp \
	Component.cpp \
	Collider.cpp \
	Camera.cpp \
	BoxCollider.cpp \
	AudioSource.cpp \
	AudioListener.cpp \
	Audio.cpp \

HEADERS += \
	Vertex.hpp \
	Type.hpp \
	Transform.hpp \
	tinyxml2.hpp \
	Time.hpp \
	Thread.hpp \
	TextureLoader.hpp \
	Texture.hpp \
	SystemInfo.hpp \
	Subsystem.hpp \
	StringUtil.hpp \
	stdafx.hpp \
	Sprite.hpp \
	ShaderLoader.hpp \
	Shader.hpp \
	sfRigidBody.hpp \
	sfDynamicsWorld.hpp \
	Settings.hpp \
	Scene.hpp \
	Rigidbody.hpp \
	Renderer.hpp \
	Rect.hpp \
	Range.hpp \
	Random.hpp \
	Property.hpp \
	Physics.hpp \
	ParticleSystem.hpp \
	Particle.hpp \
	Operators.hpp \
	Object.hpp \
	Network.hpp \
	Mesh.hpp \
	MathUtil.hpp \
	Material.hpp \
	Magnet.hpp \
	Light.hpp \
	Joint.hpp \
	Input.hpp \
	HingeJoint.hpp \
	FileManager.hpp \
	Entity.hpp \
	Engine.hpp \
	Console.hpp \
	Component.hpp \
	Collider.hpp \
	Camera.hpp \
	BoxCollider.hpp \
	BitField.hpp \
	AudioSource.hpp \
	AudioListener.hpp \
	Audio.hpp \
	Archive.hpp \

OTHER_FILES += \
	MainPage.dox \
	Doxyfile

