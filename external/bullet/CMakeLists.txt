project(bullet)

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

# Includes
include_directories(
	"src"
)
