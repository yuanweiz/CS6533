cmake_minimum_required (VERSION 2.8)
find_package (GLUT)
find_package (OpenGL)
find_package (GLEW)

include_directories ("../include")
add_library (glwrap glsupport.cpp Timer.cpp Uniform.cpp LuaConfig.cpp LuaTable.cpp Attribute.cpp Program.cpp)
target_link_libraries (glwrap GLEW glut GL )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set (CMAKE_CXX_FLAGS "-Wall -std=c++11" )
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0" )
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG" )
