if(__FIND_OPENGL_CMAKE__)
  return()
endif()
set(__FIND_OPENGL_CMAKE__ TRUE)


enable_language(C CXX)

find_package(PkgConfig REQUIRED)
set(OPENGL_FOUND "OPENGL-NOTFOUND")

# On debian, pkg-config finds the MESA libGL.so, lets try using NVidia's if available.
find_library(OPENGL_gl_LIBRARY libnvidia-cfg.so.1 PATH_SUFFIXES nvidia)
if(OPENGL_gl_LIBRARY)
  string(REPLACE "libnvidia-cfg.so.1" "libGL.so.1" OPENGL_gl_LIBRARY "${OPENGL_gl_LIBRARY}")

  pkg_check_modules(OPENGL REQUIRED x11 egl)
  list(INSERT OPENGL_LIBRARIES 0 :${OPENGL_gl_LIBRARY})
else()
  pkg_check_modules(OPENGL REQUIRED x11 gl egl)
endif()
