cmake_minimum_required(VERSION 3.10)
project(Root)

add_subdirectory(src)
add_subdirectory(demo)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT demo)