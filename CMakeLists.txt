cmake_minimum_required (VERSION 2.8)
find_package (GLUT)
find_package (OpenGL)
find_package (GLEW)

include_directories ("include")

add_subdirectory ( "src_wraplib" )
#add_subdirectory ( "./examples/first")
#add_subdirectory ( "./examples/colorful_square")
#add_subdirectory ( "./examples/cube")
add_subdirectory ( "./examples/lighting")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set (CMAKE_CXX_FLAGS "-Wall -std=c++11" )
set (CMAKE_CXX_FLAGS_DEBUG "-g3 -O0" )
set (CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" )
