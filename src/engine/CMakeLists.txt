project(engine)

add_executable(engine main.cpp)

install(TARGETS engine RUNTIME DESTINATION bin)
