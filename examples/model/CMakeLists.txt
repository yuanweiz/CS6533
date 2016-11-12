project(model)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
set (LIBS glut GLEW GL glwrap lua)

set (CMAKE_CXX_COMPILER "clang++")

set (CMAKE_CXX_FLAGS "-Wall -std=c++11"	)
include_directories ("../../include")

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}	-g3	-O0" )
set (CMAKE_CXX_FLAGS_RELEASE	"${CMAKE_CXX_FLAGS_RELEASE}	-O2 -DNDEBUG" )

add_executable (model model.cpp Light.cpp)
add_definitions(-DCURRENT_DIR="${PROJECT_SOURCE_DIR}")
target_link_libraries (model ${LIBS})
