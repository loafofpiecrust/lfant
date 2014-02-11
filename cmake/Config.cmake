message("Config file called")
#get_filename_component(ROOT "./" REALPATH)

set(ARCH "x64")
set(ARCH "x64" CACHE STRING "Architecture to build for. (x86/x64)")
set(COMPILER "gcc" CACHE STRING "Compiler to use")

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

if(PLATFORM STREQUAL "")
	message(FATAL_ERROR "This platform is not supported.")
endif()

message("Platform: ${PLATFORM}")

set(CMAKE_BUILD_TYPE Release)
set(BASE_FLAGS "-O3 -Wall -D__STRICT_ANSI__ -fPIC")
if(NOT ANDROID)
	set(BASE_FLAGS "${BASE_FLAGS} -m${ARCH_OPTION}")
endif()
if(NOT WIN32)
#	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()
set(CMAKE_C_FLAGS "${BASE_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${BASE_FLAGS} -std=gnu++11 -Wno-invalid-offsetof -g -Wno-overloaded-virtual")

# RPathing
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_SHARED_LINKER_FLAGS "-m${ARCH_OPTION} -Wl,-rpath,'$ORIGIN' -Wl,-rpath,'$ORIGIN/lib' -Wl,-rpath-link,. -Wl,--no-undefined")
if(UNIX)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,origin -g")
endif()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

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