find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
set (LIBS glut GL )

set (CMAKE_CXX_COMPILER "clang++")

set (CMAKE_CXX_FLAGS "-Wall -std=c++11"	)
include_directories ("../../include")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}	-g3	-O0" )
set (CMAKE_CXX_FLAGS_RELEAS	"${CMAKE_CXX_FLAGS_RELEASE}	-O2" )

#ADD_CUSTOM_TARGET(debug ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Debug ${PROJECT_SOURCE_DIR})
#ADD_CUSTOM_TARGET(release ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Release ${PROJECT_SOURCE_DIR})

add_executable (first main.cpp)
target_link_libraries (first ${LIBS})
