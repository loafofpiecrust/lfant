message("Config file called")
#get_filename_component(ROOT "./" REALPATH)

set(ARCH "x64")
set(ARCH "x64" CACHE STRING "Architecture to build for. (x86/x64/arm)")
set(COMPILER "gcc" CACHE STRING "Compiler to use")
set(DEBUG ON CACHE BOOL "Build with debug symbols")

#set(ANDROID_NDK_PATH "" CACHE STRING "Root path of your android ndk")
set(ANDROID_TOOLCHAIN "" CACHE STRING "Toolchain name to use for android building")

# Desktop setup
if(ANDROID_TOOLCHAIN STREQUAL "")
	if(ARCH STREQUAL "x64")
		set(ARCH_OPTION "64")
	endif()
	if(ARCH STREQUAL "x86")
		set(ARCH_OPTION "32")
	endif()
	if(ARCH STREQUAL "arm")
		set(ARCH_OPTION "cpu=arm7")
	endif()

	message("Arch: m${ARCH_OPTION}")

	set(PLATFORM "")
	if(UNIX)
		if(APPLE)
			set(PLATFORM "macosx")
		else()
			set(PLATFORM "linux")
		endif()
	else()
		if(WIN32)
			set(PLATFORM "windows")
		endif()
	endif()

# Android setup
else()
	set(ANDROID ON)
	set(PLATFORM "android")

	if(UNIX)
		if(APPLE)
		else()
			set( ANDROID_NDK_HOST_SYSTEM_NAME "linux-x86" )
		endif()
	else()
		if(WIN32)
		endif()
	endif()

#	set(ANDROID_NDK ${ANDROID_NDK})
#	set(ANDROID_NDK_TOOLCHAIN_ROOT ${ANDROID_NDK_PATH}/toolchains/${ANDROID_TOOLCHAIN})
#	set(ANDROID_NDK_TOOLCHAIN_BIN "${ANDROID_NDK_TOOLCHAIN_ROOT}/prebuilt/linux-x86/arm-linux-androideabi/bin")
	set(CMAKE_TOOLCHAIN_FILE ${LFANT}/cmake/android.toolchain.cmake)
	set(ANDROID_NATIVE_API_LEVEL "android-10")
endif()

# Allow usage of clang on windows
if(PLATFORM STREQUAL "windows" AND COMPILER STREQUAL "clang")
#	include(CMakeExtraBootstrap)
	include(extra/Platform/Windows-Clang-C)
endif()

if(PLATFORM STREQUAL "")
	message(FATAL_ERROR "This platform is not supported.")
endif()

message("Platform: ${PLATFORM}")

if(COMPILER STREQUAL "gcc")
	set(CMAKE_C_COMPILER "gcc")
	set(CMAKE_CXX_COMPILER "g++")
endif()
if(COMPILER STREQUAL "clang")
	set(CMAKE_C_COMPILER "clang")
	set(CMAKE_CXX_COMPILER "clang++")
endif()

set(CMAKE_BUILD_TYPE Release)
set(BASE_FLAGS "-Wall -D__STRICT_ANSI__")
if(NOT ANDROID)
	set(BASE_FLAGS "${BASE_FLAGS} -m${ARCH_OPTION}")
endif()
if(NOT WIN32)
        set(BASE_FLAGS "${BASE_FLAGS} -fPIC")
endif()

if(DEBUG)
	set(BASE_FLAGS "${BASE_FLAGS} -O3 -g")
else()
	set(BASE_FLAGS "${BASE_FLAGS} -O3")
endif()

set(CMAKE_C_FLAGS "${BASE_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "-std=c++11 ${BASE_FLAGS} -Wno-invalid-offsetof -Wno-overloaded-virtual")

# RPathing
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(BASE_FLAGS "-m${ARCH_OPTION} -Wl,-rpath,'$ORIGIN' -Wl,-rpath,'$ORIGIN/lib' -Wl,-rpath-link,. -Wl,--no-undefined")
if(UNIX)
	set(BASE_FLAGS "-Wl,-z,origin ${BASE_FLAGS}")
endif()

if(DEBUG)
	set(BASE_FLAGS "${BASE_FLAGS} -g")
endif()

set(CMAKE_SHARED_LINKER_FLAGS "${BASE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${BASE_FLAGS}")

if(ANDROID)
	set(BIN "android/bin")
else()
	set(BIN "bin/${ARCH}/${PLATFORM}")
endif()

set(LIBRARY_OUTPUT_PATH ${ROOT}/${BIN})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

message("output: ${LIBRARY_OUTPUT_PATH}")

if(NOT ROOT STREQUAL LFANT AND NOT EXISTS "${LIBRARY_OUTPUT_PATH}/lib")
	if(PLATFORM STREQUAL "linux")
		execute_process(
			COMMAND ln -s "${LFANT}/${BIN}" "${LIBRARY_OUTPUT_PATH}/lib"
			)
	endif()
endif()


# Add a case for VCC?


if(PLATFORM STREQUAL "android")
	message("On Android, using special gcc, ${CMAKE_C_COMPILER}")
#	set(CMAKE_C_COMPILER "gcc")
#	set(CMAKE_CXX_COMPILER "g++")
endif()


if(PLATFORM STREQUAL "windows" AND CMAKE_C_COMPILER STREQUAL "clang")

	message("Windows clang")

	find_library(MINGW_ARCH_DIR libglu32.a)
	get_filename_component(MINGW_ARCH_DIR "${MINGW_ARCH_DIR}" PATH)
	set(MINGW_ARCH_DIR "${MINGW_ARCH_DIR}/..")

	get_filename_component(MINGW_ROOT "${MINGW_ARCH_DIR}/.." REALPATH)

	file(GLOB MINGW_VER_DIR "${MINGW_ROOT}/lib/gcc/*/*/libgcc.a")
	get_filename_component(MINGW_VER_DIR "${MINGW_VER_DIR}" PATH)

	message("root dir: ${MINGW_ROOT}, ver_dir: ${MINGW_VER_DIR}")

	link_directories(
		${MINGW_ARCH_DIR}/lib
		${MINGW_ARCH_DIR}/lib32
		${MINGW_VER_DIR}
	)

	include_directories(
		${MINGW_ARCH_DIR}/include
		${MINGW_ARCH_DIR}/include/c++
	)

endif()

if(PLATFORM STREQUAL "windows")

#	link_directories(
	#	C:/Windows/System32
	#	C:/Windows/SysWOW64
#	)

endif()

if(PLATFORM STREQUAL "linux")
	link_directories(
		/usr/lib/x86_64-linux-gnu/mesa
	)
endif()
