# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)

# Choose an appropriate compiler prefix
#set(CMAKE_TOOLCHAIN_PREFIX "llvm-")

set(TOOLCHAIN_DIR "C:/tdm-gcc-64/x86_64-w64-mingw32")


# which compilers to use for C and C++
#find_program(CMAKE_RC_COMPILER  NAMES ${CMAKE_TOOLCHAIN_PREFIX}-windres)
#find_program(CMAKE_C_COMPILER   NAMES gcc)
#find_program(CMAKE_CXX_COMPILER NAMES g++)
#find_program(CMAKE_ASM_COMPILER NAMES ${CMAKE_TOOLCHAIN_PREFIX}-as)

set(CMAKE_C_COMPILER   "clang"   "-isystem ${TOOLCHAIN_DIR}/include")
set(CMAKE_CXX_COMPILER "clang++" "-isystem ${TOOLCHAIN_DIR}/include")

find_program(CMAKE_AR NAMES llvm-ar)
#set(CMAKE_AR "llvm-ar")
#message("${CMAKE_AR}")
set(CMAKE_OBJDUMP "llvm-objdump")
set(CMAKE_LINKER "g++")

# here is the target environment located
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR}/${CMAKE_TOOLCHAIN_PREFIX})

#set(CMAKE_LINK_LIBRARY_SUFFIX ".dll.a")
#set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.dll;.dll.a;.a")

#message("link suffix: ${CMAKE_LINK_LIBRARY_SUFFIX}")

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_CROSS_COMPILING TRUE)
