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

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

# Includes
include_directories(
	"src"
)
