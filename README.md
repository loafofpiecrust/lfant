lfant
=========
A cross-platform, OpenGL game engine that uses an Entity-Component model, fairly similar Unity3D, where
Entities have a list of Components that they can access at any given time. These components "add"
functionality to the owner Entity. Examples of Components are Rigidbody, Inventory,
MeshRenderer, ParticleSystem, etc. This allows the easy manipulation of what Entities can do at runtime. All external libraries not included in a default install of each OS are included.

Latest development: https://github.com/loafofpiecrust/lfant

Building
============
Binaries here are not always up-to-date with the code, so it is suggested to build them.
lfant uses primarily CMake for building. Some IDEs support the use of a CMakeLists.txt file directly
as a project.
To compile using the CMake CLI, first install it. On Linux distributions using aptitude for package
management (eg. Ubuntu):
```bash
sudo apt-get install cmake
```
On Windows or OSX, download it from here: http://cmake.org/cmake/resources/software.html

We use the LLVM/Clang compiler, but it builds perfectly fine with the latest GCC. GCC comes with
most/all Linux distros by default, but Windows users will need to install MinGW (I suggest TDM-GCC).
On Mac OSX, XCode comes with both GCC and Clang.

Then, create a folder to build to. This directory's location is irrelevant, but I suggest 'lfant/build'.
Open a terminal in your build directory, and do this (substituting '~/lfant' with your directory):
```bash
mkdir ~/lfant/build
cd ~/lfant/build
cmake ../
make
```
This will build the engine, it's dependencies, and the example(s). The engine and dependencies will build
to lfant/bin(ARCH)/(PLATFORM), eg. bin64/linux. Examples output to their respective binary
folders, like lfant/examples/galaga/bin64/linux.

This may switch to premake, once it is more usable for this project.

Android
=========
Install the android sdk and ndk, and download the platform tools for whichever android
platform you want, here I use API level 10, or Gingerbread.
```bash
mkdir ~/lfant/android/build
cd ~/lfant/android/build

export ANDROID_NDK=$HOME/android/ndk
cmake -DANDROID_TOOLCHAIN=x86-4.8 ../..
make

sudo apt-get install ant
mkdir ~projects/proj/android/build
cd ~/projects/proj/android/build
cmake -DANDROID_TOOLCHAIN=x86-4.8 ../..
make

cd ../
$HOME/android/sdk/tools/android update project -t "android-10" -p . -s
$HOME/android/ndk/ndk-build
```

To build the apk:
```bash
ant debug
```
or
```bash
ant debug install
```

Creating a project (WIP)
============================
To create a new project based on lfant, the script 'new-project.sh'(WIP) in lfant root can be run. When the editor is available, it will allow the creation and management of projects.

Running Examples
====================
Each example builds a launcher with it, residing alongside its respective project binary. Simply run
this launcher, and the "Launch()" function in the game library will be called.
