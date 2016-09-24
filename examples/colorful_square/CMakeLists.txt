find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
set (LIBS glut GLEW GL glwrap)

set (CMAKE_CXX_COMPILER "clang++")

set (CMAKE_CXX_FLAGS "-Wall -std=c++11"	)
include_directories ("../../include")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}	-g3	-O0" )
set (CMAKE_CXX_FLAGS_RELEASE	"${CMAKE_CXX_FLAGS_RELEASE}	-O2 -DNDEBUG" )

add_executable (colorful colorful.cpp)
target_link_libraries (colorful ${LIBS})

add_executable (single_colored single_colored.cpp)
target_link_libraries (single_colored ${LIBS})

add_executable (texture texture.cpp )
target_link_libraries (texture ${LIBS})

add_executable (time time.cpp )
target_link_libraries (time ${LIBS})
