project(assimp)

# Source Dir
file(GLOB SRC_FILES
	code/*.cpp
	contrib/ConvertUTF/*.cpp
	contrib/unzip/*.c
	)

file(GLOB HEADER_FILES
	include/*.h
	code/*.h
	)

list(REMOVE_ITEM SRC_FILES
	${PROJECT_SOURCE_DIR}/code/OgreImporter.cpp
	${PROJECT_SOURCE_DIR}/code/OgreMaterial.cpp
	${PROJECT_SOURCE_DIR}/code/OgreMesh.cpp
	${PROJECT_SOURCE_DIR}/code/OgreSkeleton.cpp
	${PROJECT_SOURCE_DIR}/code/IRRLoader.cpp
	${PROJECT_SOURCE_DIR}/code/IRRMeshLoader.cpp
	${PROJECT_SOURCE_DIR}/code/IRRShared.cpp
	${PROJECT_SOURCE_DIR}/code/IFCBoolean.cpp
	${PROJECT_SOURCE_DIR}/code/IFCCurve.cpp
	${PROJECT_SOURCE_DIR}/code/IFCGeometry.cpp
	${PROJECT_SOURCE_DIR}/code/IFCLoader.cpp
	${PROJECT_SOURCE_DIR}/code/IFCMaterial.cpp
	${PROJECT_SOURCE_DIR}/code/IFCOpenings.cpp
	${PROJECT_SOURCE_DIR}/code/IFCProfile.cpp
	${PROJECT_SOURCE_DIR}/code/IFCReaderGen.cpp
	${PROJECT_SOURCE_DIR}/code/IFCUtil.cpp
	${PROJECT_SOURCE_DIR}/code/ColladaParser.cpp
	${PROJECT_SOURCE_DIR}/code/ColladaLoader.cpp
	${PROJECT_SOURCE_DIR}/code/XGLLoader.cpp
	)

# Output Library Info

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

# Includes
include_directories(
	include
	${ROOT}/external/boost
	)

add_definitions(
	-DASSIMP_BUILD_NO_IRRMESH_IMPORTER
	-DASSIMP_BUILD_NO_IRR_IMPORTER
	-DASSIMP_BUILD_NO_OGRE_IMPORTER
	-DASSIMP_BUILD_NO_IFC_IMPORTER
	-DASSIMP_BUILD_NO_COLLADA_IMPORTER
	-DASSIMP_BUILD_NO_XGL_IMPORTER
	)

target_link_libraries(${PROJECT_NAME}
	z
	)

if(PLATFORM STREQUAL "linux")
target_link_libraries(${PROJECT_NAME}
	rt
	)
endif()
