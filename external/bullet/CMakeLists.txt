project(bullet)

set(PROJECT_OUTPUT_DIR ./lib)

# Source Dir
file(GLOB_RECURSE SRC_FILES
	"src/BulletCollision/*.cpp"
	"src/BulletDynamics/*.cpp"
	"src/BulletSoftBody/*.cpp"
	"src/LinearMath/*.cpp"
	)

file(GLOB_RECURSE HEADER_FILES
	"src/BulletCollision/*.h"
	"src/BulletDynamics/*.h"
	"src/BulletSoftBody/*.h"
	"src/LinearMath/*.h"
	)

list(REMOVE_ITEM SRC_FILES
	${PROJECT_SOURCE_DIR}/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp
	)

if(EXISTS "${PROJECT_SOURCE_DIR}/src")
	execute_process(
		COMMAND svn update src
		)
else()
	execute_process(
		COMMAND svn co http://svn.boost.org/svn/boost/trunk/libs src
		)
endif()

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

add_definitions(
	-DBT_USE_DOUBLE_PRECISION
)

# Includes
include_directories(
	"src"
)

if(PLATFORM STREQUAL "windows")
target_link_libraries(${PROJECT_NAME}
	stdc++
)
endif()

#add_subdirectory(d)
