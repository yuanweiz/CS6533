cmake_minimum_required (VERSION 2.8)
find_package (GLUT)
find_package (OpenGL)
find_package (GLEW)
#find_package (GLU) //GLU are now obsolete
add_executable( template glsupport.cpp main.cpp)
target_link_libraries ( template glut GL GLEW GLU)
include_directories ("../../include")

add_library (glsupport glsupport.cpp)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set (CMAKE_CXX_FLAGS "-Wall -std=c++11" )
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0" )
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2" )
