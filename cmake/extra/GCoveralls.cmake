if(__GCOVERALLS_CMAKE__)
  return()
endif()
set(__GCOVERALLS_CMAKE__ TRUE)


set(GCOVERALLS_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function(enable_gcoveralls)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage" PARENT_SCOPE)

  configure_file(${GCOVERALLS_LIST_DIR}/coveralls-upload.in ${CMAKE_BINARY_DIR}/coveralls-upload @ONLY)
endfunction()
