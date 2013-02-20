## ShadowFox Engine
===================
A so-far small OpenGL game engine. It uses an Entity-Component model, somewhat similar Unity3D, where 
Entities have a list of Components that they can access at any given time. These components "add" 
functionality to the owner Entity (albeit not directly). Examples of Components are Rigidbody, Inventory,
MeshRenderer, ParticleSystem, etc. This allows the easy manipulation of what Entities can do at runtime.

External Library Usage:
 - Bullet Physics
 - Boost (portions)
 - GLEW
 - GLFW
 - GLM
 - Squirrel (Scripting Language)
 - TinyXML2
 - CEGUI
 
Visit our website: http://www.shadowfoxltd.com

Latest unstable development: https://github.com/babybluesedan/shadowfox-engine

Latest mostly-stable development: https://github.com/shadowfox-studios/shadowfox-engine

## Installation
===============
Binaries here are not always up-to-date with the code, so it is suggested to build them. To build,
though, you will need to download RakNet here: http://www.jenkinssoftware.com/download.html, likely
under the Hobbyist license.
ShadowFox Engine uses primarily CMake for building. Qt Creator is the main IDE used, too.
To compile with Qt Creator is simple, you just add the root CMakeLists.txt as a project and build. 
To compile using the CMake CLI, first install it. On Linux distributions using aptitude for package
management (eg. Ubuntu):
```bash
sudo apt-get install cmake
```
On Windows or OSX, download it from here: http://cmake.org/cmake/resources/software.html

Then, create a folder to build to. For example, shadowfox-engine/build. This directory's location doesn't
matter all that much in the end. Open a terminal in your build directory, and do this (substituting
"~/shadowfox-engine" with your SFE directory):
```bash
cmake ~/shadowfox-engine
make
```
This will build the engine, it's dependencies, and the example(s). The engine and dependencies will build
to shadowfox-engine/bin(ARCH)/(PLATFORM), eg. bin64/linux. Examples output to their respective binary
folders, like shadowfox-engine/examples/galaga/bin64/linux.

## Running Examples
===================
Each example builds a launcher with it, residing alongside its respective project binary. Simply run
this launcher, and the "Launch()" function in the game library will be called.
